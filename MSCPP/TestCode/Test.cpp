#include "stdafx.h"


#include <coroutine>
#include <exception>
#include <optional>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <future>
#include <type_traits>

#include <vector>        // <-- 추가
#include <queue>         // <-- 추가
#include <functional>    // <-- 추가



namespace Test
{
    namespace AsyncTask
    {
        // C# void 형 Task 대체용 단위 타입
        struct TaskVoid
        {
        };

        //=========================================================================================
        // C# ThreadPool 유사한 역할
        //  - 전역 싱글톤으로 사용: ThreadPool::instance().enqueue(...)
        //=========================================================================================
        class ThreadPool
        {
        public:
            // 싱글톤 인스턴스 얻기
            static ThreadPool& instance()
            {
                static ThreadPool pool;
                return pool;
            }

            // 작업 큐에 함수 추가
            void enqueue(std::function<void()> job)
            {
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    tasks_.push(std::move(job));
                }
                cv_.notify_one();
            }

        private:
            ThreadPool()
            {
                unsigned n = std::thread::hardware_concurrency();
                if (n == 0) n = 4;

                for (unsigned i = 0; i < n; ++i)
                {
                    workers_.emplace_back([this]()
                    {
                        for (;;)
                        {
                            std::function<void()> task;

                            {
                                std::unique_lock<std::mutex> lk(mutex_);
                                cv_.wait(lk, [this] {
                                    return stop_ || !tasks_.empty();
                                });

                                if (stop_ && tasks_.empty())
                                    return;

                                task = std::move(tasks_.front());
                                tasks_.pop();
                            }

                            task(); // 실제 작업 실행
                        }
                    });
                }
            }

            ~ThreadPool()
            {
                {
                    std::lock_guard<std::mutex> lk(mutex_);
                    stop_ = true;
                }
                cv_.notify_all();

                for (auto& th : workers_)
                {
                    if (th.joinable())
                        th.join();
                }
            }

            ThreadPool(const ThreadPool&) = delete;
            ThreadPool& operator=(const ThreadPool&) = delete;

        private:
            std::vector<std::thread>        workers_;
            std::queue<std::function<void()>> tasks_;
            std::mutex                      mutex_;
            std::condition_variable         cv_;
            bool                            stop_ = false;
        };

        //=========================================================================================
        // ThreadPool 로 컨텍스트를 넘기는 awaiter
        //   사용 예: co_await switchToThreadPool();
        //   => 현재 코루틴은 suspend 되고, ThreadPool 워커 스레드에서 resume 됨
        //=========================================================================================
        inline auto switchToThreadPool()
        {
            struct Awaiter
            {
                bool await_ready() const noexcept { return false; }

                void await_suspend(std::coroutine_handle<> h) const
                {
                    ThreadPool::instance().enqueue([h]() {
                        h.resume();
                    });
                }

                void await_resume() const noexcept {}
            };

            return Awaiter{};
        }

        //=========================================================================================
        // C# Task<T> 유사한 코루틴 기반 Task (non-void 전용)
        //=========================================================================================
        template <typename T>
        class Task
        {
            static_assert(!std::is_void_v<T>,
                "Task<void> 대신 Task<Test::TaskVoid> 를 사용하세요.");

        public:

            //=========================================================================================
            // Task<T> 코루틴의 상태·결과·예외·continuation 을 관리하는 핵심 컨트롤 블록
            //=========================================================================================
            struct promise_type
            {
                // 이 promise_type 을 가진 코루틴을 조종하는 핸들 타입
                using handle_type = std::coroutine_handle<promise_type>;

                // co_return 으로 되돌려 줄 실제 결과값 저장소
                std::optional<T>      value_;

                // 코루틴 내부에서 발생한 예외 저장소
                std::exception_ptr    exception_;

                // 이 Task<T>를 co_await 하고 기다리고 있는 "상위 코루틴" 핸들
                // - await_suspend() 에서 설정
                // - final_suspend() 에서 깨움 (resume)
                std::coroutine_handle<> continuation_{};


                // 코루틴이 끝날 때, 우리를 기다리던 쪽(continuation)을 깨워줌
                // final_suspend() 가 반환할 awaiter 타입
                struct final_awaiter
                {
                    // 항상 false:
                    // - 코루틴이 끝날 때 한 번은 무조건 suspend 지점을 거치게 해서
                    //   그 시점에 continuation 을 깨우는 로직을 실행할 수 있게 함
                    bool await_ready() noexcept { return false; }

                    // 코루틴이 "마지막 suspend 지점"에 도달했을 때 호출됨
                    // h: 이 코루틴 자체의 핸들
                    void await_suspend(handle_type h) noexcept
                    {
                        auto& p = h.promise();

                        // 누군가 이 Task 를 co_await 하고 있다면
                        // 그 상위 코루틴(continuation_)을 다시 실행(resume)
                        if (p.continuation_)
                        {
                            p.continuation_.resume();
                        }
                    }

                    // final_suspend 이후 재개될 때 특별히 돌려줄 값은 없음
                    void await_resume() noexcept {}
                };


                // 1) 호출자에게 돌려줄 객체
                //    - 코루틴이 "생성"될 때 최초 한 번 호출됨
                //    - 외부에서 보게 될 Task<T> 인스턴스를 만들어서 리턴
                Task get_return_object() noexcept
                {
                    // from_promise(*this):
                    //   - 현재 promise 인스턴스로부터 해당 코루틴의 핸들(handle_type)을 생성
                    //   - 그 핸들을 들고 있는 Task 를 하나 만들어서 호출자에게 되돌려 줌
                    return Task{ handle_type::from_promise(*this) };
                }

                // 2) 시작/종료 시점 제어
                // initial_suspend:
                //   - 코루틴이 처음 호출될 때 "바로 실행할지" 결정
                //   - std::suspend_never: 프레임 생성 후 즉시 코루틴 본문 실행
                //   - C# async 메서드처럼 "호출하면 바로 비동기 실행 시작" 느낌
                std::suspend_never initial_suspend() noexcept { return {}; }

                // final_suspend:
                //   - 코루틴이 끝날 때 어떤 awaiter 를 통해 마무리할지 결정
                //   - 여기서는 위에서 정의한 final_awaiter 를 사용
                //   - final_awaiter 안에서 continuation 을 resume 하도록 구현되어 있음
                final_awaiter final_suspend() noexcept { return {}; }


                // 3) return 처리
                // co_return v; 가 호출될 때 불리는 함수
                //   - 결과값을 value_ 에 저장해 둔다
                void return_value(T v) noexcept
                {
                    value_.emplace(std::move(v));
                }                
                // co_return 이란
                //   - 코루틴 이용하는 함수내에서 쓰는 전용 return 문
                //   - 값(또는 void/완료)을 promise_type 에 전달하고 코루틴을 종료시키는 역할
                /*
                    Task<int> Foo()
                    {
                        // ...
                        co_return 10;
                    }

                    // p: 이 코루틴의 promise_type 인스턴스라고 가정
                    // p.return_value(10);     // 결과 저장
                    // 그 다음 final_suspend() 로 넘어가서
                    // final_awaiter 가 continuation(우리를 co_await 한 쪽)을 깨움
                */


                // 4) 예외 처리
                // 코루틴 내부에서 예외가 바깥으로 빠져나가면 호출됨
                //   - std::current_exception() 으로 예외를 캡처해서 exception_ 에 저장
                //   - 나중에 await_resume() 시점에서 재던짐(std::rethrow_exception)
                void unhandled_exception() noexcept
                {
                    exception_ = std::current_exception();
                }
            };

            using handle_type = std::coroutine_handle<promise_type>;

            Task() noexcept = default;

            explicit Task(handle_type h) noexcept : coro_(h) {}

            Task(Task&& other) noexcept : coro_(other.coro_)
            {
                other.coro_ = nullptr;
            }

            Task& operator=(Task&& other) noexcept
            {
                if (this != &other)
                {
                    if (coro_)
                        coro_.destroy();
                    coro_ = other.coro_;
                    other.coro_ = nullptr;
                }
                return *this;
            }

            Task(const Task&) = delete;
            Task& operator=(const Task&) = delete;

            ~Task()
            {
                if (coro_)
                    coro_.destroy();
            }

            bool is_ready() const noexcept
            {
                return !coro_ || coro_.done();
            }

            //=========================================================================================
            // operator co_await - Task<T>를 co_await 시 실제 대기 동작(awaiter)을 정의
            //=========================================================================================
            auto operator co_await() noexcept
            {
                // 실제로 await 동작을 수행하는 awaiter 타입
                struct awaiter
                {
                    // 이 Task가 감싸고 있는 실제 코루틴 핸들
                    // (promise_type 에 접근하거나 resume() 할 때 사용)
                    handle_type coro_;

                    // 1) await_ready:
                    //    - true  이면: 바로 완료된 것으로 보고 suspend 하지 않음
                    //    - false 이면: await_suspend()를 호출해서 현재 코루틴을 잠시 중단
                    bool await_ready() noexcept
                    {
                        // 코루틴 핸들이 없거나 (!coro_)
                        // 이미 코루틴이 끝난 상태(coro_.done() == true)면
                        // 더 기다릴 필요가 없으므로 즉시 완료 처리.
                        return !coro_ || coro_.done();
                    }

                    // 2) await_suspend:
                    //    - await_ready() 가 false 인 경우 호출됨
                    //    - 여기서 "현재 코루틴(상위 코루틴)"을 어떻게 중단하고,
                    //      우리가 감싸고 있는 Task 코루틴을 어떻게 시작/재개할지 정의
                    void await_suspend(std::coroutine_handle<> awaiting) noexcept
                    {
                        // awaiting: 지금 co_await 를 호출한 "상위" 코루틴의 핸들
                        auto& p = coro_.promise();

                        // (1) continuation 등록
                        //     - 이 Task 코루틴이 끝났을 때 다시 깨워야 할 코루틴을 기록
                        //     - final_suspend() 에서 p.continuation_.resume() 으로 깨우게 됨
                        p.continuation_ = awaiting;

                        // (2) 실제 Task 코루틴 실행 시작 / 재개
                        //     - initial_suspend() 가 suspend_never 이면 이미 돌고 있을 수도 있지만,
                        //       여기서는 "Task 코루틴을 한 번 돌려본다"는 의미로 resume() 호출
                        coro_.resume();
                    }

                    // 3) await_resume:
                    //    - Task 쪽 코루틴이 끝난 뒤(또는 이미 끝나 있었던 경우)
                    //      co_await 표현식의 결과값을 상위 코루틴에게 돌려주는 부분
                    T await_resume()
                    {
                        auto& p = coro_.promise();

                        // Task 실행 중 예외가 발생했다면 여기서 재던짐
                        // 상위 코루틴에서는 try/catch 로 잡을 수 있음
                        if (p.exception_)
                            std::rethrow_exception(p.exception_);

                        // promise_type 에 저장해 두었던 결과값을 move 로 반환
                        // (복사 비용 최소화 / 소유권 이전)
                        return std::move(*(p.value_));
                    }
                };

                // Task<T> 객체를 co_await 하면:
                //  - 컴파일러가 이 operator co_await() 를 호출해서 awaiter 를 만들고
                //  - 그 awaiter 에 대해 await_ready / await_suspend / await_resume 를 호출함
                return awaiter{ coro_ };
            }

            handle_type native_handle() const noexcept { return coro_; }

        private:
            handle_type coro_{}; // {} : value-initialization(값 초기화), C++11부터 지원하는 in-class 멤버 기본 초기화 문법
                                 // handle_type coro_ = handle_type{};  // 동일한 의미
        };

        //=========================================================================================
        // C# Task.Delay() 유사한 함수로, awaitable 객체 정의
        //      co_await delayTask(500ms); 처럼 사용
        //=========================================================================================
        struct DelayAwaiter
        {
            std::chrono::milliseconds duration;

            bool await_ready() const noexcept
            {
                return duration.count() <= 0;
            }

            void await_suspend(std::coroutine_handle<> h) const
            {
                std::thread([h, d = duration]() {
                    std::this_thread::sleep_for(d);
                    h.resume();
                }).detach();
            }

            void await_resume() const noexcept {}
        };

        inline DelayAwaiter delayTask(std::chrono::milliseconds d)
        {
            return DelayAwaiter{ d };
        }

        //=========================================================================================
        // C# Task.Run() 유사한 함수로, ThreadPool 에서 f 실행 후 결과 반환
        //   auto t = runTask([] { return heavy(); });
        //=========================================================================================
        template <typename F, typename R = std::invoke_result_t<F>>
        AsyncTask::Task<R> runTask(F&& f)
        {
            auto worker = [fn = std::forward<F>(f)]() -> Task<R>
            {
                // 1) packaged_task 생성
                std::packaged_task<R()> pt(fn);
                auto fut = pt.get_future();

                // 2) move-only packaged_task 를 shared_ptr 로 감싸기
                auto taskPtr = std::make_shared<std::packaged_task<R()>>(std::move(pt));

                // 3) ThreadPool 에 job 등록
                //    - 람다는 taskPtr 을 복사해서 캡처 (shared_ptr 복사 → OK)
                ThreadPool::instance().enqueue(
                    [taskPtr]() mutable
                {
                    // ThreadPool 워커 스레드에서 실제 작업 실행
                    (*taskPtr)();
                });

                // 4) 이 코루틴은 future 결과를 받아서 co_return
                co_return fut.get();
            };

            // worker() 코루틴을 await 해서 runTask 의 결과로 반환
            co_return co_await worker();
        }

        //=========================================================================================
        // C# Wait() 유사한 함수 (주의: 데드락/블로킹 위험 !!!)
        //=========================================================================================
        template <typename T>
        T syncWait(Task<T> task)
        {
            std::mutex              m;
            std::condition_variable cv;
            bool                    done = false;
            std::optional<T>        result;
            std::exception_ptr      ex;

            // task 를 co_await 한 뒤, 완료되면 condition_variable을 깨워주는 코루틴
            auto waiter = [&]() -> Task<TaskVoid>
            {
                try
                {
                    result = co_await task;
                }
                catch (...)
                {
                    ex = std::current_exception();
                }

                {
                    std::lock_guard lk(m);
                    done = true;
                }
                cv.notify_one();
                co_return TaskVoid{};
            };

            auto t = waiter(); // initial_suspend = never 이라 바로 실행

            {
                std::unique_lock lk(m);
                cv.wait(lk, [&] { return done; });
            }

            if (ex)
                std::rethrow_exception(ex);

            return std::move(*result);
        }

    }


    AsyncTask::Task<int> AddAsync(int a, int b)
    {
        // C#: await Task.Delay(10);
        co_await AsyncTask::delayTask(std::chrono::milliseconds(10));

        co_return a + b;
    }

    AsyncTask::Task<int> HeavyCalcAsync()
    {
        // C#: return await Task.Run(() => Heavy());
        co_return co_await AsyncTask::runTask([] {
            // 이 안은 ThreadPool 워커 스레드에서 실행
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return 123;
        });
    }

    AsyncTask::Task<AsyncTask::TaskVoid> ExampleAsync()
    {
        std::cout << "Example start (thread id = "
            << std::this_thread::get_id() << ")\n";

        int v1 = co_await AddAsync(1, 2);
        int v2 = co_await HeavyCalcAsync(); // ThreadPool 에서 heavy 작업

        std::cout << "Sum: " << (v1 + v2) << " (thread id = "
            << std::this_thread::get_id() << ")\n";

        co_return AsyncTask::TaskVoid{};
    }

    AsyncTask::Task<AsyncTask::TaskVoid> asyncWithSwitchToThreadPool()
    {
        std::cout << "before hop thread = " << std::this_thread::get_id() << "\n";

        // 여기까지는 메인 스레드에서 실행 중이라고 가정
        co_await AsyncTask::switchToThreadPool();

        // 이 아래부터는 ThreadPool 워커 스레드에서 실행
        std::cout << "after hop thread = " << std::this_thread::get_id() << "\n";

        // CPU heavy 작업
        for (int i = 0; i < 100000000; ++i) { 
            co_await HeavyCalcAsync();
        }

        co_return AsyncTask::TaskVoid{};
    }

    void Run()
    {
        // 비-async 진입점에서 전체 async 파이프라인을 한 번에 기다림
        AsyncTask::syncWait(ExampleAsync());

        int r = AsyncTask::syncWait(AddAsync(10, 20));
        std::cout << "sync_wait result: " << r << "\n";

        asyncWithSwitchToThreadPool();
    }
}