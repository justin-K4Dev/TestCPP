#include "stdafx.h"


#include <coroutine>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <stop_token>


namespace TaskWithThreadPool
{

//--------------------------------------------------------------------------------------------------
// 간단한 스레드 풀
//--------------------------------------------------------------------------------------------------
class SimpleThreadPool {
public:
    explicit SimpleThreadPool(size_t n = std::thread::hardware_concurrency())
        : _stop(false) // 스레드풀 종료 플래그 초기화(false = 실행중)
    {
        if (n == 0) n = 4; // worker 벡터 capacity 확보(재할당 줄이기)
        _workers.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            _workers.emplace_back([this] { worker_loop(); });
            // 워커 스레드 생성 및 처리할 로직 설정
            // worker_loop()에서 큐의 job을 처리
        }
    }

    ~SimpleThreadPool() { shutdown(); }
    // 스레드풀 객체 파괴 시 안전하게 종료

    void shutdown() {
        bool expected = false;
        if (!_stop.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) return;
        // _stop을 false->true로 한 번만 전환. 이미 종료 중이면 바로 리턴

        _cv.notify_all(); // 대기 중인 워커들 깨움(종료 유도)

        for (auto& t : _workers) if (t.joinable()) t.join();
        // 모든 워커 스레드 join (종료까지 대기)

        _workers.clear(); // 워커 요소 모두 제거
    }

    void schedule(std::coroutine_handle<> h, bool /*high_priority*/ = false) {
        // 외부에서 "코루틴 재개(resume)" 작업을 스레드풀에 넣는 함수

        if (!h) return;
        {
            std::lock_guard lk(_mtx); // 큐 보호 락

            _q.push([h]() mutable { h.resume(); }); // 큐에 "resume job" 등록
        }
        _cv.notify_one(); // 워커 하나 깨워서 job 처리
    }

private:
    void worker_loop() {

        // 워커 스레드 메인 루프: 큐에서 job을 pop해 실행
        while (!_stop.load(std::memory_order_acquire)) {

            std::function<void()> job; // 실행할 작업(콜러블)
            {
                std::unique_lock lk(_mtx); // 큐 보호 락

                _cv.wait(lk, [&] {
                    // 작업이 생기거나 stop될 때까지 대기
                    return _stop.load(std::memory_order_acquire) || !_q.empty(); 
                });

                if (_stop.load(std::memory_order_acquire) && _q.empty()) break;
                // 종료 플래그가 켜지고 큐도 비었으면 루프 탈출(스레드 종료)

                job = std::move(_q.front()); // job 가져오기
                _q.pop();
            }

            job(); // job 실행 (반드시 락 밖에서 실행 !!!, 데드락 방지 !!!)
        }
    }

    std::atomic<bool> _stop;                // 종료 여부(워커 루프 종료 조건)
    std::mutex _mtx;                        // 큐 보호용 뮤텍스
    std::condition_variable _cv;            // 큐 변화/종료 알림
    std::queue<std::function<void()>> _q;   // 작업 큐(람다/함수)
    std::vector<std::thread> _workers;      // 워커 스레드 객체들
};

static SimpleThreadPool& globalPool() {
    static SimpleThreadPool p(4);   // 전역 기본 풀(4워커)
    return p;                       // 싱글턴처럼 사용
}

//--------------------------------------------------------------------------------------------------
// TimerService (dedicated timer thread => no worker occupancy)
//--------------------------------------------------------------------------------------------------
class TimerService {
public:
    // steady_clock::now() 값은 시스템 시간 변경과 무관하게 계속 앞으로만 증가
    using Clock = std::chrono::steady_clock;    // monotonic 증가 시계 (타임아웃 체크 목적)
    using TimePoint = Clock::time_point;        // 시간 포인트 타입

    TimerService() : _stop(false), _th([this] { run(); }) {}

    ~TimerService() {
        {
            std::lock_guard lk(_mtx);
            _stop = true; // 타이머 스레드 종료 설정
        }
        _cv.notify_one(); // 타이머 스레드 깨움
        if (_th.joinable()) _th.join(); // 타이머 스레드 종료 대기
    }

    template<class Rep, class Period>
    void schedule_after(std::chrono::duration<Rep, Period> d, std::function<void()> cb) {
        // "지금으로부터 d 후 실행" 등록 API
        schedule_at(Clock::now() + std::chrono::duration_cast<Clock::duration>(d), std::move(cb));
    }

private:
    struct Item {
        TimePoint tp;               // 실행 예정 시각
        std::function<void()> cb;   // 실행할 콜백 함수
        uint64_t seq;               // 동일 시각 tie-breaker(안정 정렬)
    };
    struct Cmp {
        bool operator()(Item const& a, Item const& b) const {
            // priority_queue는 "큰 것"이 top이므로 비교를 뒤집어 min-heap처럼 사용
            if (a.tp != b.tp) return a.tp > b.tp;       // 더 늦은 tp가 뒤로 가도록
            return a.seq > b.seq;                       // seq로 tie-break
        }
    };

    void schedule_at(TimePoint tp, std::function<void()> cb) {
        // 특정 시각에 실행되는 이벤트 등록
        {
            std::lock_guard lk(_mtx);
            _pq.push(Item{ tp, std::move(cb), _seq++ });    // pq에 아이템 push
        }
        _cv.notify_one();   // 타이머 스레드 깨움
    }

    void run() {
        // 타이머 전용 스레드 루프: pq의 가장 빠른 이벤트 시각까지 sleep/wait
        std::unique_lock lk(_mtx);
        for (;;) {
            if (_stop) return;  // 종료 요청이면 스레드 종료

            if (_pq.empty()) {
                // 이벤트가 없으면 새 이벤트 또는 stop까지 대기
                _cv.wait(lk, [&] { return _stop || !_pq.empty(); });
                continue;
            }

            auto now = Clock::now();        // 현재 시각
            auto next_tp = _pq.top().tp;    // 다음 이벤트 시각
            if (now < next_tp) {
                // 다음 이벤트까지 기다림(또는 stop)
                _cv.wait_until(lk, next_tp, [&] { return _stop; });
                continue;
            }

            // 실행 시각이 되었으니 pop해서 실행
            auto item = std::move(_pq.top());
            _pq.pop();

            lk.unlock();    // 콜백은 락 밖에서 실행(중요!)
            item.cb();      // 등록된 콜백 수행
            lk.lock();      // 다시 락 획득하여 루프 계속
        }
    }

    std::mutex _mtx;                // pq 보호
    std::condition_variable _cv;    // 이벤트 등록/stop 알림
    std::priority_queue<Item, std::vector<Item>, Cmp> _pq;  // 이벤트 우선순위 큐(가장 빠른 것 top)
    bool _stop;                     // 타이머 스레드 종료 플래그
    std::thread _th;                // 타이머 스레드
    uint64_t _seq = 0;              // tie-break용 시퀀스
};

static TimerService& globalTimer() {
    static TimerService t;          // 전역 타이머 서비스(1개)
    return t;
}

//--------------------------------------------------------------------------------------------------
// Exceptions
//--------------------------------------------------------------------------------------------------
struct TaskTimeoutException : std::runtime_error { using std::runtime_error::runtime_error; };
// WaitAsync가 타임아웃으로 끝났음을 알리는 예외 타입

struct TaskCanceledException : std::runtime_error { using std::runtime_error::runtime_error; };
// WaitAsync가 취소(stop_token)로 끝났음을 알리는 예외 타입

//--------------------------------------------------------------------------------------------------
// 1) Promise Return Base (UNCHANGED - do not modify)
//--------------------------------------------------------------------------------------------------
template <typename T, typename Derived>
struct promise_return_base {
    std::optional<T> result;        // 코루틴 반환값 저장(단 한 번 set)
    void return_value(T v) { result.emplace(std::move(v)); }
    // co_return v; 를 만나면 컴파일러가 promise.return_value(v) 호출 -> optional에 저장
};

template <typename Derived>
struct promise_return_base<void, Derived> {
    void return_void() {}
    // co_return; 를 만나면 promise.return_void() 호출
};

//--------------------------------------------------------------------------------------------------
// ContinuationSlot: once-only resume ticket (race-safe)
//--------------------------------------------------------------------------------------------------
enum class WaitWin : int { Completed = 0, Timeout = 1, Canceled = 2 };
// WaitAsync 경쟁에서 먼저 처리될 것인지(완료/타임아웃/취소)

struct ContinuationSlot {
    std::atomic<void*> awaiting_addr{ nullptr };
    // 대기 중 코루틴(handle)의 address를 저장. exchange로 "한 번만" 가져가게 만든다.
    std::atomic<int> win{ -1 };
    // 먼저 처리될 것인지 기록(-1=미결정). await_resume에서 이 값을 보고 예외/정상 결정

    void set(std::coroutine_handle<> h) noexcept {
        awaiting_addr.store(h.address(), std::memory_order_release);
        // 대기 코루틴 주소 저장(나중에 resume해야 함)
    }

    std::coroutine_handle<> take() noexcept {
        void* p = awaiting_addr.exchange(nullptr, std::memory_order_acq_rel);
        // exchange로 "한 번만" 주소를 가져감 (나머지 경쟁자는 nullptr 받음)

        return p ? std::coroutine_handle<>::from_address(p) : std::coroutine_handle<>{};
    }

    bool try_fire(WaitWin w, std::coroutine_handle<>& out) noexcept {
        auto h = take();                                // 내가 continuation을 가져갈 수 있나?
        if (!h) return false;                           // 이미 다른 이벤트가 가져감(패배)
        win.store((int)w, std::memory_order_release);   // 승자 기록
        out = h;                                        // resume할 handle 반환
        return true;                                    // 성공(처리 가능)
    }
};

//--------------------------------------------------------------------------------------------------
// Result storage helper (so SharedState<T> is unified for void/non-void)
//--------------------------------------------------------------------------------------------------
template<typename T>
struct state_result {
    std::optional<T> result;        // non-void의 경우 결과 저장 공간
};

template<>
struct state_result<void> {};       // void는 결과 저장 필드 없음

//==================================================================================================
// SharedState<T>: unified
//==================================================================================================
template<typename T>
struct SharedState : state_result<T> {
    SimpleThreadPool* pool = nullptr;           // 이 Task가 사용할 풀(없으면 globalPool)

    std::exception_ptr ex;                      // 코루틴에서 던진 예외 저장

    std::atomic<bool> completed{ false };       // 완료 여부(Wait()의 조건)
    std::mutex done_mtx;                        // condition_variable 보호용 mutex
    std::condition_variable done_cv;            // Wait()에서 block/unblock 용

    std::mutex wait_mtx;                                    // waiters 벡터 보호
    std::vector<std::shared_ptr<ContinuationSlot>> waiters; // co_await/WaitAsync 대기자들
    std::atomic<bool> completed_flag{ false };              // "이미 완료됨" 빠른 경로 플래그(늦게 등록된 waiter 처리)

    void notify_all_waiters_completed() {
        completed_flag.store(true, std::memory_order_release);

        std::vector<std::shared_ptr<ContinuationSlot>> local;
        {
            std::lock_guard lk(wait_mtx);
            local.swap(waiters);    // waiters를 로컬로 빼서 락 짧게
        }

        SimpleThreadPool* p = pool ? pool : &globalPool();  // 사용할 풀 선택

        // ALWAYS schedule (no inline resume)
        for (auto& s : local) {
            std::coroutine_handle<> ch;
            if (s && s->try_fire(WaitWin::Completed, ch)) {
                p->schedule(ch, false);     // inline resume 금지: 항상 스케줄링
            }
        }
    }

    void add_waiter(const std::shared_ptr<ContinuationSlot>& s) {
        SimpleThreadPool* p = pool ? pool : &globalPool();  // 사용할 풀 선택

        if (completed_flag.load(std::memory_order_acquire)) {
            // 이미 완료된 상태면 waiter 등록 없이 즉시 깨움
            std::coroutine_handle<> ch;
            if (s && s->try_fire(WaitWin::Completed, ch)) {
                p->schedule(ch, false);
            }
            return;
        }

        std::lock_guard lk(wait_mtx);   // waiters 추가를 락으로 보호

        if (completed_flag.load(std::memory_order_acquire)) {
            // 락 획득 후 재확인(완료와 경합 방지)
            std::coroutine_handle<> ch;
            if (s && s->try_fire(WaitWin::Completed, ch)) {
                p->schedule(ch, false);
            }
            return;
        }

        waiters.push_back(s);   // 아직 미완료면 대기자 목록에 추가
    }
};

//==================================================================================================
// Task<T> (COPYABLE) - multi-consumer
//==================================================================================================
template<typename T>
class Task {
public:
    struct promise_type;                                        // 코루틴 promise 선언
    using state_type = SharedState<T>;                          // 공유 상태 타입
    using handle_type = std::coroutine_handle<promise_type>;    // 코루틴 핸들 타입(실제로는 promise 내부에서만 사용)

    static constexpr auto InfiniteTimeout = std::chrono::steady_clock::duration::max();
    // WaitAsync에서 무한 타임아웃을 표현하는 값(Timeout.Infinite 유사)

    Task() = default;       // 빈 Task
    explicit Task(std::shared_ptr<state_type> st) : _st(std::move(st)) {}
    // shared-state를 받아 Task 생성 (copyable의 핵심)

    // COPYABLE
    Task(const Task&) = default;            // shared_ptr 복사 => multi-consumer 가능
    Task& operator=(const Task&) = default;

    Task(Task&&) noexcept = default;        // move도 기본
    Task& operator=(Task&&) noexcept = default;

    bool valid() const noexcept { return (bool)_st; }   // shared-state가 있으면 유효

    // Wait (blocking)
    void wait() const {
        if (!_st) throw std::runtime_error("invalid Task"); // 상태 없으면 오류

        std::unique_lock lk(_st->done_mtx); // condition_variable 대기용 락
        _st->done_cv.wait(lk, [&] {         // 완료될 때까지 block
            return _st->completed.load(std::memory_order_acquire); 
        });

        if (_st->ex) std::rethrow_exception(_st->ex); // 코루틴 예외를 동기 호출자에게 재throw
    }

    // Result: returns const-ref (multi-consumer, avoids copy requirement)
    template<typename U = T, typename std::enable_if<!std::is_void_v<U>, int>::type = 0>
    const U& Result() const {
        wait();                     // 완료까지 대기
        if (!_st->result.has_value()) throw std::runtime_error("Task has no result");
        return *_st->result;        // 결과를 const-ref로 반환(복사 비용 회피)
    }

    // co_await (multi awaiters)
    auto operator co_await() const noexcept {
        struct awaiter {
            std::shared_ptr<state_type> st; // shared-state를 캡쳐(수명 보장)

            bool await_ready() noexcept {
                // 완료됐다면 suspend 없이 즉시 진행
                return st->completed.load(std::memory_order_acquire);
            }

            std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) noexcept {
                // 미완료면 continuation 등록
                auto slot = std::make_shared<ContinuationSlot>();
                slot->set(awaiting);                                // 현재 awaiting 코루틴 저장
                st->add_waiter(slot);                               // shared-state에 waiter로 등록
                return std::noop_coroutine();                       // "내가 직접 재개하지 않음" 을 의미
            }

            decltype(auto) await_resume() {
                // 재개된 뒤 호출. 결과/예외를 호출자에게 전달
                if (st->ex) std::rethrow_exception(st->ex);

                if constexpr (std::is_void_v<T>) {
                    return; // void는 반환값 없음
                }
                else {
                    if (!st->result.has_value()) throw std::runtime_error("Task has no result");
                    return (const T&)(*st->result); // const T& 반환
                }
            }
        };

        return awaiter{ _st };      // awaiter 생성 반환
    }

    // WaitAsync(timeout, token) -> Task<T>
    template<class Rep, class Period>
    Task<T> waitAsync(std::chrono::duration<Rep, Period> timeout, std::stop_token token = {}) const {
        // duration을 steady_clock::duration으로 맞춘 뒤 내부 구현 코루틴 호출
        return waitAsyncImpl(std::chrono::duration_cast<std::chrono::steady_clock::duration>(timeout), token);
    }

    Task<T> waitAsync(std::stop_token token = {}) const {
        // 무한 타임아웃 버전(Timeout.Infinite 느낌)
        return waitAsyncImpl(InfiniteTimeout, token);
    }

private:
    Task<T> waitAsyncImpl(std::chrono::steady_clock::duration timeout, std::stop_token token) const {
        // WaitAsync는 “코루틴”으로 구현됨: 완료/timeout/cancel 경쟁 후 결과를 co_return
        auto st = _st;  // shared-state 캡쳐
        if (!st) throw std::runtime_error("invalid Task");

        struct WaiterAwaiter {
            std::shared_ptr<state_type> st;                 // 기다릴 대상 상태
            std::chrono::steady_clock::duration timeout;
            std::stop_token token;

            std::shared_ptr<ContinuationSlot> slot;         // 경쟁 제어용 슬롯(once-only resume)
            std::optional<std::stop_callback<std::function<void()>>> stopcb;
            // stop_token 취소 콜백 RAII. WaiterAwaiter가 살아있는 동안만 등록 유지.

            bool await_ready() noexcept {
                // 이미 완료라면 suspend 없이 바로 끝냄
                return st->completed.load(std::memory_order_acquire);
            }

            std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting) {
                // 아직 미완료면 completion/timeout/cancel 경쟁 등록
                slot = std::make_shared<ContinuationSlot>();
                slot->set(awaiting);            // 현재 awaiting 코루틴 저장
                st->add_waiter(slot);           // 완료 시 깨울 waiter 등록

                SimpleThreadPool* pool = st->pool ? st->pool : &globalPool();

                // timeout
                if (timeout != Task::InfiniteTimeout) {
                    globalTimer().schedule_after(timeout, [slot = slot, pool] {
                        std::coroutine_handle<> ch;
                        if (slot && slot->try_fire(WaitWin::Timeout, ch)) {
                            pool->schedule(ch, false); // 타임아웃 승자면 awaiting을 깨움(스케줄링)
                        }
                    });
                }

                // cancel
                if (token.stop_possible()) {
                    stopcb.emplace(token, std::function<void()>([slot = slot, pool] {
                        std::coroutine_handle<> ch;
                        if (slot && slot->try_fire(WaitWin::Canceled, ch)) {
                            pool->schedule(ch, false);  // 취소 승자면 awaiting을 깨움(스케줄링)
                        }
                    }));
                }

                return std::noop_coroutine();   // 실제 재개는 completion/timeout/cancel에서 수행
            }

            void await_resume() {
                // 깨어난 뒤, 누가 승자인지에 따라 예외/정상 처리
                int w = slot ? slot->win.load(std::memory_order_acquire) : (int)WaitWin::Completed;

                if (w == (int)WaitWin::Timeout)  throw TaskTimeoutException("WaitAsync timeout");
                if (w == (int)WaitWin::Canceled) throw TaskCanceledException("WaitAsync canceled");
                
                // Completed면 그냥 통과
            }
        };

        co_await WaiterAwaiter{ st, timeout, token }; // 경쟁 await(완료/timeout/cancel 중 하나)

        // Completed won
        if (st->ex) std::rethrow_exception(st->ex); // 원본 Task의 예외가 있으면 재throw

        if constexpr (std::is_void_v<T>) {
            co_return;  // void는 값 없이 종료
        }
        else {
            if (!st->result.has_value()) throw std::runtime_error("Task has no result");
            co_return *st->result;
            // 값을 “복사”해서 WaitAsync Task의 promise.result에 저장
        }
    }

private:
    std::shared_ptr<state_type> _st; // Task의 핵심: 공유 상태를 소유(shared_ptr)

public:
    //=============================================================================================
    // promise_type (IMPORTANT: no return_value/return_void defined here!)
    //=============================================================================================
    struct promise_type : public promise_return_base<T, promise_type> {
        // promise_return_base가 T에 따라 return_value 또는 return_void를 제공.

        std::shared_ptr<state_type> st; // 공유 상태(결과/예외/대기자/완료 플래그)

        promise_type() 
            : st(std::make_shared<state_type>()) {
            // 기본 생성: shared-state 생성
        }

        template<typename... Args>
        promise_type(SimpleThreadPool& p, Args&&...)
            : st(std::make_shared<state_type>()) {
            st->pool = &p;      // 생성자 인자로 풀을 주입하면 그 풀 사용
        }

        Task get_return_object() {
            return Task{ st };  // 호출자에게 copyable Task 반환(shared_ptr 공유)
        }

        // Start automatically on pool
        struct start_awaiter {
            SimpleThreadPool* pool = nullptr;               // 시작 시 사용할 풀

            bool await_ready() noexcept { return false; }
            // false => initial_suspend에서 “반드시 suspend”한 뒤 await_suspend 호출

            void await_suspend(handle_type h) noexcept {
                SimpleThreadPool* p = pool ? pool : &globalPool();
                p->schedule(h, false); // 첫 resume를 풀에 넣어 자동 시작(호출 즉시 시작처럼 보이게)
            }
            
            void await_resume() noexcept {}
            // initial_suspend에서 깨어난 뒤 특별히 할 일 없음
        };

        start_awaiter initial_suspend() noexcept { return start_awaiter{ st->pool }; }
        // 코루틴 생성 직후 initial_suspend에서 start_awaiter가 사용됨.
        // 결과적으로 코루틴 본문은 "바로 실행”이 아니라 “풀에 스케줄 후 실행".

        // Final: publish completion, publish result (for non-void), wake waiters, destroy frame
        struct final_awaiter {
            bool await_ready() noexcept { return false; }
            // final_suspend에서도 항상 await_suspend를 타서 “신호/깨우기/파괴”를 수행

            std::coroutine_handle<> await_suspend(handle_type h) noexcept {
                auto& pr = h.promise();     // promise 참조
                auto st = pr.st;            // shared-state 복사(로컬로 들고있어 수명 보장)

                // publish result into shared-state (multi-consumer)
                if constexpr (!std::is_void_v<T>) {
                    // pr.result is from promise_return_base<T,...>
                    if (pr.result.has_value()) {
                        st->result = std::move(pr.result); // shared-state에 결과를 옮김(한 번만)
                    }
                }

                // signal completion
                st->completed.store(true, std::memory_order_release);   // 완료 플래그 publish
                st->done_cv.notify_all();                               // Wait()로 block된 스레드들 깨움
                st->notify_all_waiters_completed();                     // co_await/WaitAsync 대기자들 깨움

                // destroy coroutine frame (state stays alive via shared_ptr)
                h.destroy();                    // 코루틴 프레임 파괴(결과는 shared-state에 남아있음)
                return std::noop_coroutine();   // continuation 없음
            }

            void await_resume() noexcept {}
            // final_suspend에서 재개될 일은 거의 없지만, 시그니처는 필요
        };

        final_awaiter final_suspend() noexcept { return {}; }
        // 코루틴 종료 시 final_awaiter 실행

        void unhandled_exception() noexcept {
            st->ex = std::current_exception();
            // 예외를 shared-state에 저장(다중 consumer에서 재throw 가능)
        }
    };
};

//=================================================================================================
// 테스트
//=================================================================================================
Task<void> task_with_fire_and_forget(SimpleThreadPool& pool, int id) {
 
    std::cout << "[job " << id << "] start\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));    // 대기
    std::cout << "[job " << id << "] end\n";

    co_return;              // void 코루틴 종료(return_void 호출됨)
}

Task<int> compute_async(SimpleThreadPool& pool, int x) {

    std::cout << "[compute_async] start x=" << x << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    std::cout << "[compute_async] end\n";

    co_return x * 2;        // int 반환(return_value 호출되어 promise.result에 저장)
}

Task<void> multi_consumer(SimpleThreadPool& pool) {
    auto t = compute_async(pool, 21);   // int Task 생성(자동으로 풀에 스케줄되어 실행 시작)

    // multi-await safe
    const int& a = co_await t;      // 첫 await: 완료되면 shared-state result 참조 받음
    const int& b = co_await t;      // 두 번째 await: 동일 결과를 다시 참조(멀티 컨슈머)
    std::cout << "multi-await refs: " << a << ", " << b << "\n";

    try {
        int v = co_await t.waitAsync(std::chrono::milliseconds(50));    // 타임아웃 50ms로 대기
        std::cout << "WaitAsync completed: " << v << "\n";
    }
    catch (const TaskTimeoutException&) {
        std::cout << "WaitAsync timeout\n";     // 50ms 안에 완료 못하면 여기
    }

    int v2 = co_await t.waitAsync(); // 무한 대기(Timeout.Infinite)
    std::cout << "WaitAsync infinite completed: " << v2 << "\n";

    // void task works
    co_await task_with_fire_and_forget(pool, 99);   // Task<void>도 동일 구조로 await 가능

    co_return;                                      // multi_consumer 종료
}

//=================================================================================================
// 테스트 엔트리
//=================================================================================================

void Test() {
    SimpleThreadPool pool(4);

    // fire-and-forget: starts automatically even if ignored
    task_with_fire_and_forget(pool, 1);     // 반환 Task를 버려도 initial_suspend가 스케줄링해서 실행됨
    task_with_fire_and_forget(pool, 2);     // 두 번째도 동일

    // Result() multiple times (returns const ref)
    {
        auto t = compute_async(pool, 10);       // Task<int> 생성/시작
        const int& r1 = t.Result();             // Wait + 결과 참조 반환
        const int& r2 = t.Result();             // 다시 Result() 호출해도 같은 shared-state 결과 참조
        std::cout << "Result twice refs: " << r1 << ", " << r2 << "\n";
    }

    {
        auto d = multi_consumer(pool);          // Task<void> 생성/시작
        d.wait();                               // 동기(blocking)로 완료까지 대기
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    pool.shutdown();
}

}//TaskWithThreadPool