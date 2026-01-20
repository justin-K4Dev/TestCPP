#include "stdafx.h"

#include <iostream>
#include <coroutine>
#include <thread>
#include <chrono>


namespace Coroutine
{
    void Coroutine_what()
    {
        /*
            📚 Coroutine (C++20)

              - Coroutine(코루틴)은 함수의 실행을 중단(suspend)하고
                나중에 재개(resume)할 수 있는 메커니즘을 제공하는 기능입니다.
              - 기존 함수 호출과 달리 스택을 유지하면서 상태를 보존합니다.

              🔹 주요 특징
                - 함수 호출 중간에 일시정지하고 값 또는 제어를 호출자에게 반환 가능
                - 이후 해당 위치부터 실행을 재개 가능
                - 비동기 처리를 위한 새로운 방식 제공 (`co_await`)
                - 반복자(generator) 스타일 구현 가능 (`co_yield`)

              🔹 도입 배경
                - 기존의 콜백 기반 비동기 처리 방식은 가독성이 낮고 유지보수가 어려움
                - Coroutine은 더 선형적이고 직관적인 흐름으로 비동기 로직을 작성할 수 있도록 도와줌

              🔹 핵심 키워드
                - `co_await`: 비동기 작업 완료를 기다리고 중단
                - `co_yield`: 값을 생성(yield)하고 중단
                - `co_return`: 코루틴 종료 및 결과 반환

              🔹 구성 요소
                1. promise_type:
                  1.1. 코루틴의 상태를 관리
                    - “코루틴 프레임의 정책/결과/continuation”을 관리
                  1.2. 결과 저장소
                    - co_return value;로 전달되는 값을 어디에 저장할지
                    - void면 완료 플래그 정도만 필요
                  1.3. 예외 저장소
                    - 코루틴 본문에서 throw가 나가면 unhandled_exception()으로 들어와서 exception_ptr에 저장
                    - 이후 await_resume()에서 재던짐
                  1.4. suspend 정책용 awaiter 종류 (언제 멈추고 언제 이어갈지)
                    - std::suspend_never: 함수 호출하자마자 코루틴이 “즉시 실행”
                    - std::suspend_always:“생성만 하고 실행은 나중에 resume”
                  1.5. 주요 맴버 함수
                    1.5.1. initial_suspend()
                      - 코루틴 시작 시점 정책
                    1.5.2. final_suspend()
                      - 코루틴 종료 직전 정책
                        여기서 흔히 continuation(부모 코루틴)을 resume/schedule함
                    1.5.3. get_return_object()
                      - 호출자에게 반환될 사용자 정의(Task<T>) 객체(코루틴 핸들 래퍼)를 만든다.
                    1.5.4. return_void()
                      - 정상적으로 완료되었고, 반환 값은 없음을 promise에 기록
                      - 그 다음 final_suspend()로 진행
                      - 아무 것도 안 하거나(done 플래그만 세팅)
                      - 완료 이벤트/상태 갱신(커스텀 Task 구현에서 종종)
                    1.5.5. return_value(T v)
                      - 정상적으로 완료되었고, 최종 결과 값 v를 promise에 저장하여
                        나중에 awaiter/호출자가 꺼낼 수 있게 만드는 함수
                      - T를 프레임 내 저장소(맴버변수)에 보관 (예: std::optional<T> result)
                      - std::variant<T, exception_ptr> 같은 상태 저장
                      - 완료 상태 갱신
                    1.5.6. unhandled_exception()
                      - 코루틴 바디 안에서 예외가 발생했는데,
                        그 예외가 코루틴 내부에서 try/catch로 처리되지 않았고,
                        코루틴 밖으로 “전파”될 수 없을 때,
                        promise_type::unhandled_exception() 호출됨
                    1.5.7. (선택) await_transform(expr)
                      - 코루틴에서 co_await expr;를 만났을 때, 컴파일러가 “그 expr을 그대로 await하지 말고,
                        promise가 변환한 결과를 await하라”라고 가로채서 변환할 수 있게 해주는 훅(hook)
                      
                2. coroutine_handle<>:
                  - “프레임을 resume/destroy하는 리모컨”
                  - coroutine_handle은 코루틴 프레임을 가리키는 얇은 핸들(pointer 비슷) 이고, 코루틴을 제어하는 API를 제공
                  - resume/destroy 동시 실행시 오류 발생 주의 !!!
                  - 같은 handle을 여러 스레드에서 동시에 resume하면 UB(Undefined Behavior)급 사고
                  - destroy 후 resume하면 access violation !!!                  
                  2.1. resume()
                    - 코루틴을 이어서 실행
                  2.2. done()
                    - 코루틴이 final_suspend에 도달했는지 여부
                    - done()이 true여도 destroy는 아직!
                  2.3. destroy()
                    - 코루틴 프레임 해제(메모리/자원 정리)
                    - 한 번만 해야 함
                  2.4. promise()
                    - promise_type에 접근

                3. awaiter / awaitable:
                  - “co_await에서 suspend/재개/결과 반환을 결정하는 프로토콜”이다.
                  - co_await 를 만나면 컴파일러가 아래 규칙으로 “awaitable → awaiter”를 뽑고, 
                    (선택)promise.await_transform(expr), awaiter의 3개 함수를 호출한다.                  
                  3.1. awaitable vs awaiter
                    3.1.1. awaitable
                      - “co_await의 대상이 될 수 있는 객체”
                        직접 await_ready/await_suspend/await_resume를 가질 수도 있고,
                        await_transform(expr) 나 operator co_await()를 제공해서 awaiter를 반환할 수도 있음
                      - 예를들어, Task 클래스는 awaitable 이고, Task 내에 Awaiter 클래스를 정의하고
                        Task 내에 operator co_await() 맴버함수 정의해서 Awaiter 를 반환해도 되며,
                        Awaiter 클래스를 단독으로 정의해도 정의해도 된다.                                 
                    3.1.2. awaiter
                      - 실제로 3개 훅을 가진 객체
                        즉, co_await X에서 X 자체가 awaiter일 수도 있고,
                        X가 awaiter를 만들어주는 “팩토리(awaitable)”일 수도 있다.
                  3.2. await_ready()
                    - 언제 suspend할지 결정
                    - true일 경우: suspend 없이 바로 await_resume()로 간다.
                    - false일 경우: await_suspend()가 호출되고 현재 코루틴이 멈춘다.
                    - “이미 결과가 준비됨”이면 true로 해서 불필요한 스케줄을 줄임
                    - 많은 Task/Result는 “완료 여부(done)”로 판단
                  3.3. await_suspend(std::coroutine_handle<> awaiting)
                    - 현재 코루틴을 멈춘 뒤, 다음에 누가/어떻게 재개할지 결정
                    - awaiting = “지금 co_await를 호출한 상위 코루틴 핸들”
                    3.3.1. 일반적인 처리
                      - promise.continuation = awaiting; (부모 저장)
                      - 작업을 큐에 넣고(work enqueue)
                      - 완료되면 continuation을 스케줄링
                    3.3.2. 변환 타입에 따른 의미 (매우 중요)
                      - await_suspend는 여러 시그니처를 가질 수 있다.
                      - 직접 awaiting.resume()를 호출하기보다
                        “executor/pool에 schedule(awaiting)”만 하고 끝내는 게 보통 안전
                      3.3.2.1. void await_suspend(handle)
                        - “무조건 suspend”
                        - 재개는 외부에서 awaiting.resume() 또는 schedule로 해줘야 함
                      3.3.2.2. bool await_suspend(handle)
                        - true 반환: suspend 됨 (외부가 재개해야 함)
                        - false 반환: suspend하지 않고 즉시 계속 진행 (바로 resume처럼 동작)
                      3.3.2.3. std::coroutine_handle<> await_suspend(handle)
                        - “대신 실행할 코루틴”을 반환
                        - 즉, awaiting 대신 반환한 핸들이 실행 흐름을 이어받는다(스케줄 전환)
                  3.4. await_resume()
                    - co_await 표현식의 결과를 돌려주는 곳
                    - 결과 값을 꺼내서 반환하거나
                      저장된 예외를 rethrow_exception 한다.
                    - await_ready() == true 였든 아니든, 최종적으로 여기를 탄다.
                    - await_resume()가 호출될 때는 “작업이 완료됐다”는 가정이므로
                      내부 상태(optional, 포인터, future 등)가 준비돼 있어야 한다.

              🔹 반환 타입
                - 일반 함수와 달리 Coroutine은 특별한 반환 타입 필요
                - 예: `Generator<T>`, `Task<T>`, `Lazy<T>` 등 (사용자가 직접 정의하거나 라이브러리 사용)

              🔹 컴파일러가 생성하는 코드
                1. 코루틴 프레임 (Coroution Frame)
                  - promise_type 객체
                  - “다음에 어디서 재개할지”를 나타내는 상태(state)
                2. 코루틴 핸들 std::coroutine_handle<promise_type>
                  - 프레임을 가리키는 경량 핸들
                  2.1. 주요 함수
                    - resume(): 다음 지점부터 실행
                    - destroy(): 프레임 해제(지역변수 소멸 포함)
                    - done(): 완료 여부
                3. 반환 객체(return object)
                  - promise_type::get_return_object()가 만들어서 반환하는 타입
                  - 사용자 타입 (Generator<T>, Task<T>, Lazy<T> ...)
                4. co_return / co_return expr
                  - promise 훅으로 넘겨 완료 결과를 프레임에 저장(혹은 다른 방식)하는 구조
                  4.1. 생성된 코드
                    - promise.return_value(expr);
                    - 완료 상태로 전이
                    - final_suspend() await
                5. co_return / co_return expr
                  - promise 훅으로 넘겨 완료 결과를 프레임에 저장(혹은 다른 방식)하는 구조
                  5.1. 생성된 코드
                    - promise.return_value(expr);
                    - 완료 상태로 전이
                    - final_suspend() await
                6. co_await expr
                  - expr : awaiter 가 넘어와야 한다 !!!
                  6.1. 생성된 코드
                    6.1.1. (선택) promise.await_transform(expr) 함수가 있다면 먼저 호출 !!!
                    6.1.2. awaiter를 얻음
                    6.1.3. awaiter.await_ready() => false 일경우
                      - 코루틴 상태 저장(다음 재개 지점 기록)
                      - awaiter.await_suspend(handle) 호출
                      - suspend(호출자에게 제어 반환)
                    6.1.3. awaiter.await_ready() => true 일경우
                      - awaiter.await_resume() 호출 후 다음 줄 진행
                7. co_yield x
                  - co_await promise.yield_value(x) 로 코드가 변환됨 !!!
                  - 따라서 promise_type에 yield_value(T) 훅이 있어야 하고,
                    그게 반환하는 awaiter의 ready/suspend/resume 프로토콜에 따라 “yield 시 멈추는지/계속 가는지”가 결정

              🔹 사용 사례
                - Generator / 이터레이터
                - async/await 비동기 처리
                - 상태기계, 중단 가능한 시뮬레이션
                - 게임 AI 루프, 서버 요청 처리기

              🔹 표준 지원
                - `<coroutine>` 헤더 필요 (컴파일러에 따라 `<experimental/coroutine>` 일 수도 있음)
                - 기본 `generator`, `task`는 표준에 포함되지 않음 → 직접 구현 또는 라이브러리 사용 필요
                - MSVC, GCC, Clang 모두 C++20 코루틴 지원 중

              🔹 예제 문법

                Generator<int> countUpTo(int max) {
                    for (int i = 1; i <= max; ++i)
                        co_yield i;
                }

                Task<int> calculateAsync(int x) {
                    int result = co_await someAsyncFunc(x);
                    co_return result * 2;
                }

            
            📚 코루틴과 멀티스레드의 차이점

              1. 개념
                - 코루틴(C++20 coroutine)
                  : 함수가 "여기까지 했으니 잠깐 멈춰" 했다가 나중에 "거기서부터 다시" 할 수 있게 하는 언어 기능.
                    실행은 결국 스레드 위에서 이루어진다. 코루틴 자체가 CPU를 직접 점유하지는 않는다.
                - 멀티스레드(std::thread, OS thread)
                  : OS가 여러 스레드를 스케줄링해서 실제로 동시에(또는 번갈아) 실행되게 하는 것.

              2. 실행 방식
                - 코루틴: 같은 스레드에서 여러 코루틴을 번갈아 실행하는 구조. "협력적 전환"에 가깝다.
                          누군가가 resume() 해주거나 스케줄러가 올려줘야 다시 돈다. 자동으로는 안 돈다.
                - 스레드: OS가 타이머/우선순위에 따라 강제로 스레드를 바꾼다(선점형). 코드가 따로 요청하지 않아도 바뀐다.

              3. 병렬성
                - 코루틴: 한 스레드 위에서만 돌면 진짜 병렬은 아니다. 논리적 동시성만 있다.
                          병렬로 하고 싶으면 결국 여러 스레드 + 그 위에 여러 코루틴을 올리는 구조로 가야 한다.
                - 스레드: 코어가 여러 개면 실제로 동시에 돌아간다. CPU 바운드 작업을 나눌 때 이점이 있다.

              4. 컨텍스트 스위칭 비용
                - 코루틴: 같은 스레드 안에서 "어떤 코루틴 프레임을 다음에 실행할까"만 바꾸면 되므로 가볍다.
                          레지스터/스택 전체를 OS에 넘기지 않는다.
                - 스레드: OS가 현재 스레드의 레지스터, 스택 포인터, 상태를 저장하고 다른 스레드 걸 복원해야 해서 상대적으로 무겁다.
                          스레드 수가 많거나 락 대기가 많으면 이 비용이 눈에 띈다.

              5. 동기화
                - 코루틴: 한 스레드 위에서 순차적으로만 재개되면 데이터 경합이 없어서 락을 안 써도 되는 경우가 많다.
                - 스레드: 서로 다른 스레드가 동시에 같은 메모리로 들어올 수 있으므로 mutex, atomic 등이 필요하다.

              6. 스케줄링 주체
                - 코루틴: 우리가 만든 스케줄러(예: AdvancedThreadPool)나 호출자가 직접 schedule(handle) 해서 실행을 이어간다.
                          "언제 돌릴지"를 사용자 코드가 결정한다.
                - 스레드: OS 스케줄러가 결정한다. 우리가 정확한 전환 시점을 통제하기 어렵다.

              7. 쓰는 자리
                - 코루틴: 많은 비동기 작업, await 체인, I/O가 많고 작업 단위가 아주 작은 경우, 태스크를 수만 개 띄우고 싶을 때.
                - 스레드: CPU를 실제로 병렬로 돌리고 싶을 때, OS 객체와 직접 대기/신호를 걸 때, 긴 연산을 분리할 때.

              8. 결론
                - 코루틴은 "스레드를 줄이면서도 구조를 비동기스럽게" 만들고 싶을 때 유용하다.
                - 멀티스레드는 "실제로 동시에 여러 코어를 쓰겠다"가 목표일 때 필요하다.
                - 두 개를 섞어서 "스레드풀 위에 코루틴 스케줄러" 형태로 만들면 
                  C# Task/async-await 비슷한 구조를 C++에서 흉내낼 수 있다.
        */
    }

    //=============================================================================================

    //---------------------------------------------------------------------------------------------
    // 코루틴 반환 타입 #1: operator co_await() 및 ADL(Argument-Dependent Lookup) 사용시
    //---------------------------------------------------------------------------------------------
    struct TaskNoTransform 
    {
        struct promise_type {
            TaskNoTransform get_return_object() noexcept { return {}; }
            
            std::suspend_never initial_suspend() noexcept { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }

            void return_void() noexcept {}

            void unhandled_exception() { std::terminate(); }
        };
    };

    //---------------------------------------------------------------------------------------------
    // 1) Member operator co_await() 함수를 사용하는 방법
    //   - expr 자체가 멤버로 Awaiter 제공
    //---------------------------------------------------------------------------------------------
    struct MemberAwaiter 
    {
        bool await_ready() const noexcept {
            std::cout << "[Member Awaiter] await_ready -> false\n";
            return false;
            // false -> 무조건 suspend로 감
        }
        bool await_suspend(std::coroutine_handle<>) const noexcept {
            std::cout << "[Member Awaiter] await_suspend (return false => no suspend)\n";
            return false;
            // 여기서 코루틴이 멈춘 상태가 된다 !!!
        }
        int await_resume() const noexcept {
            std::cout << "[Member Awaiter] await_resume -> 20\n";
            return 20;
            // resume() 되어 co_await가 끝날 때 호출됨
        }
    };

    struct MemberToken 
    {
        // ⭐ 핵심: 멤버 operator co_await
        MemberAwaiter operator co_await() const noexcept {
            std::cout << "[Member] MemberToken::operator co_await() selected\n";
            return {};
        }
    };

    TaskNoTransform awaitable_by_member() {
        std::cout << "\n=== awaitable_by_member ===\n";
        int v = co_await MemberToken{};
        std::cout << "awaitable_by_member got v=" << v << "\n";
    }

    //---------------------------------------------------------------------------------------------
    // 2) promise_type::await_transform(expr) 함수를 사용하는 방법
    //   - promise가 expr을 "가로채서" Awaiter로 변환
    //   - 이 코루틴 타입 안에서의 co_await expr에 대해, 먼저 await_transform()이 적용됨
    //---------------------------------------------------------------------------------------------
    struct TransformToken {};

    struct TransformAwaiter {
        bool await_ready() const noexcept {
            std::cout << "[Transform Awaiter] await_ready -> false\n";
            return false;
            // false -> 무조건 suspend로 감
        }
        bool await_suspend(std::coroutine_handle<>) const noexcept {
            std::cout << "[Transform Awaiter] await_suspend (return false => no suspend)\n";
            return false;
            // 여기서 코루틴이 멈춘 상태가 된다 !!!
        }
        int await_resume() const noexcept {
            std::cout << "[Transform Awaiter] await_resume -> 30\n";
            return 30;
            // resume() 되어 co_await가 끝날 때 호출됨
        }
    };

    struct TaskWithTransform
    {
        struct promise_type {
            TaskWithTransform get_return_object() noexcept { return {}; }
            std::suspend_never initial_suspend() noexcept { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() noexcept {}
            void unhandled_exception() { std::terminate(); }

            // ⭐ 핵심: await_transform()
            TransformAwaiter await_transform(TransformToken) noexcept {
                std::cout << "[promise] await_transform(TransformToken) selected\n";
                return {};
            }

            // (옵션) 다른 타입은 그대로 통과시키고 싶으면 이런 식으로도 가능
            template <class U>
            U&& await_transform(U&& u) noexcept {
                return std::forward<U>(u);
            }
        };
    };

    TaskWithTransform awaitable_by_transform() {
        std::cout << "\n=== awaitable_by_transform ===\n";
        int v = co_await TransformToken{};
        std::cout << "awaitable_by_transform got v=" << v << "\n";
    }

    //---------------------------------------------------------------------------------------------
    // 3) ADL(Argument-Dependent Lookup) 형태로 사용시
    //   - 비멤버 operator co_await(expr) 로 Awaiter 반환
    //   - expr의 "연관 네임스페이스"에서 operator co_await 를 ADL로 찾음
    //---------------------------------------------------------------------------------------------
    namespace by_ADL {

        struct AdlToken {};

        // 실제 awaiter
        struct AdlAwaiter {
            bool await_ready() const noexcept {
                std::cout << "[ADL Awaiter] await_ready -> false\n";
                return false;
            }
            bool await_suspend(std::coroutine_handle<>) const noexcept {
                std::cout << "[ADL Awaiter] await_suspend (return false => no suspend)\n";
                return false; // suspend 하지 않고 즉시 계속 진행
            }
            int await_resume() const noexcept {
                std::cout << "[ADL Awaiter] await_resume -> 10\n";
                return 10;
            }
        };

        // ⭐ 핵심: 비멤버 operator co_await (ADL로 발견됨)
        inline AdlAwaiter operator co_await(AdlToken) noexcept {
            std::cout << "[ADL] operator co_await(AdlToken) selected\n";
            return {};
        }

    } // namespace by_ADL

    TaskNoTransform awaitable_by_ADL() {
        std::cout << "\n=== awaitable_by_ADL ===\n";
        int v = co_await by_ADL::AdlToken{};
        std::cout << "awaitable_by_ADL got v=" << v << "\n";
    }

    void coroutine_to_awaiter_use()
    {
        awaitable_by_ADL();
        awaitable_by_transform();
        awaitable_by_member();
    }


    //=============================================================================================

    struct ResultInt 
    {
        struct promise_type {
            std::optional<int> value;

            ResultInt get_return_object() {
                return ResultInt{ std::coroutine_handle<promise_type>::from_promise(*this) };
            }
            std::suspend_never initial_suspend() noexcept { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }

            void return_value(int v) noexcept { value = v; }
            void unhandled_exception() { std::terminate(); }
        };

        std::coroutine_handle<promise_type> h;

        // awaitable 기능 없음! (operator co_await 없음)
        int get() {
            if (!h) throw std::runtime_error("no handle");
            // 필요하면 여기서 resume() 로 실행을 진행시킬 수도 있음 (설계에 따라)
            // while(!h.done()) h.resume();

            auto& p = h.promise();
            if (!p.value) throw std::runtime_error("no value yet");
            return *p.value;
        }

        ~ResultInt() { if (h) h.destroy(); }
    };

    ResultInt foo() {
        co_return 42;
    }

    void coroutine_to_non_awaiter_use()
    {
        auto r = foo();
        int v = r.get();
        // co_await r; // ❌ 컴파일 에러 (awaitable 아님)

        system("pause");
    }

    //=============================================================================================

    struct Generator // awaiterable 역할
    {
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type 
        {
            int current_value;

            // co_yield i; 컴파일시 co_await promise_type.yield_value(i); 코드로 변환되어 생성됨 !!!
            // std::suspend_always (awaiter) 반환
            // 따라서 아래의 함수가 호출 !!!
            std::suspend_always yield_value(int value) 
            {
                current_value = value;
                return {}; // std::suspend_always(awaiter) 반환
            }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            Generator get_return_object()
            {
                return Generator{ handle_type::from_promise(*this) };
            }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };

        handle_type handle;
        explicit Generator(handle_type h) : handle(h) {}
        Generator(const Generator&) = delete;
        Generator(Generator&& other) noexcept : handle(other.handle) { other.handle = nullptr; }
        ~Generator() { if (handle) handle.destroy(); }

        // iterator-like 인터페이스 제공
        bool next() 
        {
            if (!handle.done()) handle.resume();
            return !handle.done();
        }

        int value() const { return handle.promise().current_value; }
    };

    /* 실제 코루틴 함수 */
    Generator counter(int from, int to)
    {
        for (int i = from; i <= to; ++i) 
        {
            co_yield i; // 중간 값을 호출자에게 전달하고 중단(suspend)
        }

        /*
            * 개념적인 이해를 돕기 위한 컴파일된 코드의 예시
        
            struct counter_frame {
                promise_type promise;
                int from;
                int to;
                int i;

                enum State { Start, AfterYield, Done } state;
                std::coroutine_handle<promise_type> h;
            };

            void resume(counter_frame* f) {
                auto h = f->h;

                switch (f->state) {
                    case Start:         goto L_start;
                    case AfterYield:    goto L_after_yield;
                    case Done:          return;
                }

                L_start:
                    // (from/to는 이미 프레임에 저장되어 있다고 가정)
                    f->i = f->from;

                    // 루프 시작
                    if (f->i > f->to) goto L_done;

                L_loop:
                    // =========================
                    // co_yield f->i;
                    // =========================
                    {
                        auto awaiter = f->promise.yield_value(f->i);    // current = i; return suspend_always{};
                        if (!awaiter.await_ready()) {                   // false
                            f->state = AfterYield;                      // 재개 지점 저장
                            awaiter.await_suspend(h);                   // suspend
                            return;                                     // 호출자(next())로 돌아감
                        }
                        awaiter.await_resume();
                    }

                L_after_yield:
                    // yield 이후로 복귀(재개)
                    ++f->i;
                    if (f->i <= f->to) goto L_loop;

                L_done:
                    f->promise.return_void();
                    f->state = Done;

                    // final_suspend co_await ...
                    auto fin = f->promise.final_suspend();
                    if (!fin.await_ready()) { fin.await_suspend(h); return; }
                    fin.await_resume();
            }        
        */
    }

    void co_yield_use()
    {
        /*
            📚 co_yield

              - 호출자에게 중간값을 전달(yield) 하며 함수 실행을 일시 중단(suspend) 시킴
              - 이후 resume 시 중단한 지점부터 계속 실행

              🔹 특징
                - 하나의 값을 반환하고 코루틴을 suspend
                - 호출자는 값을 받아 사용한 후 `resume()`을 통해 다음 단계로 진행
                - 내부적으로 promise_type의 `yield_value()` 함수 호출
                - 아래의 예시 컴파일시 우측의 코드로 생성됨 !!!
                  co_yield 1;   =>    std::suspend_always awaiter = promise.yield_value(1);
                                      co_await awaiter;
                  
        */
        {
            auto gen = counter(1, 3);
                // 코루틴 프레임 생성 + initial_suspend에서 멈춤 (본문 아직 미실행)
            
            while (gen.next()
                    // 1차 실행
                        // 내부에서 handle.resume()
                        // -> 코루틴이 실행되며 for문 진입
                        // -> co_yield 1 도달
                        // -> promise.yield_value(1) 호출
                        // -> current = 1 실행
                        // -> suspend_always로 멈춤
                    
                    // 2차 실행
                        // resume 계속
                        // -> 다음 반복
                        // -> co_yield 2
                        // -> current = 2 실행 후 멈춤

                    // n차 실행
                        // 2차와 동일한 수행
                ) {
                std::cout << gen.value()
                    // 1차 실행 
                        // current 읽음 => 1
                    // 2차 실행
                        // current 읽음 => 2
                    // n차 실행
                        // current 읽음 => n
                    << " ";
                    
            }
            std::cout << std::endl; // 출력: 1 2 3

            system("pause");
        }
    }

    //=============================================================================================

    //---------------------------------------------------------------------------------------------
    // 1) return_void() 함수를 사용한 경우
    //---------------------------------------------------------------------------------------------

    struct VoidTask 
    {
        struct promise_type 
        {
            VoidTask get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };
    };

    VoidTask return_void_use() 
    {
        std::cout << "[co_return_use()] 작업 시작\n";

        // 예시 작업: 루프 처리
        for (int i = 1; i <= 3; ++i) {
            std::cout << "  - 작업 #" << i << " 처리 중...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 모의 대기
        }

        std::cout << "[co_return_use()] 작업 완료\n";
        co_return; // void 반환
    }

    //---------------------------------------------------------------------------------------------
    // 2) return_value() 함수를 사용한 경우
    //---------------------------------------------------------------------------------------------

    struct IntTask
    {
        struct promise_type
        {
            std::optional<int> value;

            IntTask get_return_object()
            {
                return IntTask{ std::coroutine_handle<promise_type>::from_promise(*this) };
            }

            // 호출 즉시 본문 실행 (eager)
            std::suspend_never initial_suspend() noexcept { return {}; }

            // 중요: 결과를 꺼낼 때까지 프레임 유지
            std::suspend_always final_suspend() noexcept { return {}; }

            void return_value(int v) noexcept
            {
                value = v; // 결과 저장
            }

            void unhandled_exception() { std::terminate(); }
        };

        std::coroutine_handle<promise_type> h{};

        explicit IntTask(std::coroutine_handle<promise_type> hh) : h(hh) {}

        IntTask(const IntTask&) = delete;
        IntTask& operator=(const IntTask&) = delete;

        IntTask(IntTask&& other) noexcept : h(other.h) { other.h = {}; }
        IntTask& operator=(IntTask&& other) noexcept
        {
            if (this != &other)
            {
                if (h) h.destroy();
                h = other.h;
                other.h = {};
            }
            return *this;
        }

        int get()
        {
            if (!h) throw std::runtime_error("IntTask: no handle");

            // initial_suspend가 suspend_never라 보통 이미 실행 완료지만,
            // 안전하게 완료까지 진행(설계 변경 시 유용)
            while (!h.done()) h.resume();

            auto& p = h.promise();
            if (!p.value) throw std::runtime_error("IntTask: no value yet");

            return *p.value;
        }

        ~IntTask()
        {
            if (h) h.destroy();
        }
    };

    IntTask return_value_use()
    {
        std::cout << "[return_value_use()] 작업 시작\n";

        int sum = 0;
        for (int i = 1; i <= 3; ++i) {
            std::cout << "  - 작업 #" << i << " 처리 중...\n";
            sum += i;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        std::cout << "[return_value_use()] 작업 완료\n";
        co_return sum; // -> promise.return_value(sum)
    }

    void co_return_use()
    {
        /*
            📚 co_return

              - 코루틴 함수에서 최종 결과를 반환하거나 종료를 명시할 때 사용됨
              - 반환 타입은 `promise_type`에서 정의한 `return_value()` 또는 `return_void()`에 위임됨

              🔹 특징
                - 일반 함수의 `return`처럼 사용되지만, 내부적으로는 promise에 결과를 전달
                - 코루틴 함수는 일반 함수와 달리 실제 반환값이 아니라 `promise_type`의 인터페이스에 따름
        */
        
        {
            // 2) 값 반환 코루틴
            auto t = return_value_use();
            int result = t.get(); // 호출자가 명시적으로 get() 호출
            std::cout << "[co_return_use()] return_value_use() 결과 = " << result << "\n";

            // 1) void 반환 코루틴
            return_void_use();
        }

        system("pause");
    }

    //=============================================================================================

    // 전방 선언: 나중에 main에서 이걸 resume할 거라 밖에 빼둠
    std::coroutine_handle<> g_saved_handle;

    //---------------------------------------------------------------------------------------------
    // 1. 기다리기 위한 awaiter !!!
    //---------------------------------------------------------------------------------------------
    struct SimpleAwaiter 
    {
        bool await_ready() const noexcept {
            // false면 "아, 일단 멈춰야겠다" 하고 suspend 단계로 감
            return false;
        }

        void await_suspend(std::coroutine_handle<> h) const noexcept {
            std::cout << "[suspending...]\n";
            // 여기서 코루틴이 멈춘 상태가 됨
            // 나중에 다시 깨우려면 이 핸들을 저장해야 함
            g_saved_handle = h;
            // 아무것도 안 하면 그냥 멈춘 채로 끝
        }

        void await_resume() const noexcept {
            // resume() 되었을 때 코루틴이 이걸 호출함
            std::cout << "[resumed!]\n";
        }
    };

    //---------------------------------------------------------------------------------------------
    // 2. 코루틴 반환
    //---------------------------------------------------------------------------------------------
    struct AwaitTask
    {
        struct promise_type {
            AwaitTask get_return_object() {
                return AwaitTask{
                    std::coroutine_handle<promise_type>::from_promise(*this)
                };
            }
            std::suspend_always initial_suspend() noexcept { return {}; } // 시작할 때도 일단 멈춰있게
            std::suspend_always final_suspend() noexcept { return {}; }   // 끝나도 멈춰있게
            void return_void() noexcept {}
            void unhandled_exception() { std::terminate(); }
        };

        std::coroutine_handle<promise_type> handle;
        explicit AwaitTask(std::coroutine_handle<promise_type> h) : handle(h) {}
        ~AwaitTask() {
            if (handle) handle.destroy();
        }
    };

    AwaitTask run_co_await()
    {
        co_await SimpleAwaiter{};
        std::cout << "work done\n";
    }

    void co_await_use()
    {
        /*
            📚 co_await

              - 비동기 작업을 기다리는(wait) 제어 흐름을 표현함
              - 호출 시 해당 작업이 완료될 때까지 코루틴을 일시 중단(suspend) 함
              - 이후 외부 조건이 만족되면 resume 되어 다시 실행을 이어감

              🔹 동작 흐름
                - co_await expr; 호출 시 다음 3단계가 호출됨 (expr은 Awaiter 또는 Awaitable)
                ① expr.await_ready()
                  - true면 즉시 진행, false면 suspend로 진입
                ② expr.await_suspend(handle)
                  - 중단 시 실행되는 코드 (예: 예약, 타이머 등록 등)
                ③ expr.await_resume()
                  - 다시 재개(resume)될 때 결과를 반환하거나 후속 로직 수행

              🔹 Awaiter 조건:
                - await_ready() → bool
                - await_suspend(coroutine_handle<>) → void/bool
                - await_resume() → 반환값
        */

        // 코루틴 하나 만듦 (아직 안 돈 상태: initial_suspend 때문에)
        AwaitTask task = run_co_await();

        // 초기 상태가 suspend니까 여기서 한 번 깨워줘야
        // 코루틴 본문이 실행돼서 "co_await SimpleAwaiter{}" 까지 감
        std::cout << "[main] resume first time\n";
        task.handle.resume();   // -> 여기서 co_await SimpleAwaiter 까지 가고, 거기서 또 suspend
                                // 그때 SimpleAwaiter::await_suspend 가 불리면서
                                // g_saved_handle 에 코루틴 핸들이 저장됨

        // 이제 코루틴은 SimpleAwaiter 때문에 멈춰 있는 상태
        // 우리가 다시 깨워줘야 await_resume() 이 호출됨
        std::cout << "[main] resume second time\n";
        if (g_saved_handle) {
            g_saved_handle.resume();  // -> 이때 await_resume() 이 호출되고, 그 다음 "work done" 출력
        }

        system("pause");
    }

    //=============================================================================================

    //---------------------------------------------------------------------------------------------
    // 1. co_await + integer
    //---------------------------------------------------------------------------------------------
    struct AwaitTransformTask 
    {
        // 코루틴 프레임 안에 생성되는 promise 타입(코루틴의 상태/훅들을 제공)
        struct promise_type {
            // 코루틴 함수가 호출될 때 호출자에게 “반환할 객체”를 만든다.
            // 아무 상태도 들고 있지 않은 빈 AwaitTransformTask를 반환 한다.
            AwaitTransformTask get_return_object() { return {}; }

            // 코루틴이 시작(프레임 생성)될 때, 즉시 코루틴 본문 실행
            std::suspend_never initial_suspend() noexcept { return {}; }
            
            // 코루틴이 종료(프레임 파괴)될 때, 즉시 종료
            std::suspend_never final_suspend() noexcept { return {}; }
            // 일반적으로는 연속 작업 / continuation / 자원수명 때문에 suspend_always를 쓰는 경우가 많다.
            
            // co_return; 처럼 void를 반환하는 코루틴일 때 호출 !!!
            void return_void() noexcept {}
            // 코루틴 본문에서 예외가 밖으로 전파될 때 호출 !!!
            void unhandled_exception() { std::terminate(); }

            // ✅ co_await expr 가 호출될 때, expr이 int면 여기로 들어옴 !!!
            auto await_transform(int ms)
            // 코루틴 본문에서 `co_await <expr>`을 만났을 때, expr에 대해 변환을 가로채는 훅.
            // 여기서는 expr 타입이 int인 경우만 오버로드로 잡는다.
            // 예) co_await 1000;  => await_transform(1000) 호출됨.
            {
                struct SleepAwaiter
                // await_transform이 반환하는 "Awaiter" 타입(코루틴이 기다릴 대상).
                // co_await는 결과적으로 awaiter.await_ready/await_suspend/await_resume를 호출한다.
                {
                    int ms; // 대기(슬립) 시간(ms)을 저장

                    // false => await_suspend() 로 넘어간다 !!!
                    bool await_ready() noexcept { return false; }
                    // true 반환시: “이미 준비됨”이라 suspend 없이 바로 진행.
                    // false 반환시: suspend 로직(await_suspend) 경로로 간다.

                    void await_suspend(std::coroutine_handle<> h)
                    // 코루틴을 중단(suspend)할 때 호출되는 훅.
                    // 일반적으로는 보통 여기서 타이머 등록, IO 등록, 스케줄러에 등록 한다.
                    // 나중에 적절한 시점에 h.resume()으로 재개 시킨다 !!!
                    {
                        std::cout << "pretend sleep " << ms << " ms\n";
                        h.resume(); // 바로 재개
                    }

                    void await_resume() noexcept {}
                    // suspend에서 깨어난 뒤(재개 후) 호출되는 훅.
                    // 보통 여기서 결과값을 반환하거나(예: I/O 결과), 예외를 던지기도 한다.

                };
                return SleepAwaiter{ ms };
                // co_await에 사용할 awaiter 객체를 생성해서 반환.
                // co_await 500; 이면 SleepAwaiter{500}이 반환되어 그 awaiter가 사용된다.
            }
        };
    };

    AwaitTransformTask await_transform_with_primitive_type_use() {
        std::cout << "A\n";
        co_await 123;   // ✅ 원래 int는 await 불가인데, AwaitTransformTask.await_transform()이 가로챔
        std::cout << "B\n";
    }

    //=============================================================================================

    //---------------------------------------------------------------------------------------------
    // 2. co_await + 사용자 정의 Task<U>
    //---------------------------------------------------------------------------------------------
    
    //---- pool -----------------------------------------------------------------------------------
    struct Pool { int id = 0; };

    Pool& globalPool() {
        static Pool p{ 999 };
        return p;
    }

    //---- Task -----------------------------------------------------------------------------------
    template <typename T = void>
    struct Task {

        // 코루틴 프레임에 저장되는 promise(코루틴의 훅/상태 제공)
        struct promise_type {
            Pool* pool_ = nullptr;

            // 코루틴 함수가 호출될 때 호출자에게 돌려줄 “리턴 오브젝트(Task)” 생성
            Task get_return_object() {
                return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
                // 현재 promise로부터 coroutine_handle을 만들고, 그것을 Task 안에 담아 반환.
            }
            
            std::suspend_never initial_suspend() noexcept { return {}; }
            // initial suspend 지점에서 중단하지 않음 => 코루틴 프레임 생성 직후 본문을 즉시 실행.

            std::suspend_never final_suspend() noexcept { return {}; }
            // final suspend 지점에서 중단하지 않음 => 코루틴이 끝나면 “마지막에 멈추지 않고” 바로 종료(프레임 파괴 가능 경로).
            // 일반적으로 continuation 처리/수명 관리 때문에 suspend_always를 쓰는 경우가 많음.

            void unhandled_exception() { std::terminate(); }
            // 코루틴 본문에서 예외가 밖으로 전파되면 여기로 들어옴.
            // 예제에서는 terminate로 즉시 종료(간단 처리).

            // T=void 용
            void return_void() noexcept {}
            // co_return; (void 반환)일 때 호출되는 훅.
            // (반환값이 없으므로 아무 것도 저장하지 않음)

            // ✅ co_await Task<U> 가 들어오면 여기로 가서 pool 자동 주입(개념)
            template <typename U>
            Task<U> await_transform(Task<U> t) {
                // 코루틴 본문에서 `co_await something`을 만났을 때,
                // 그 `something`이 Task<U> 타입이면 이 오버로드가 선택됨.
                // 즉, `co_await Task<U>`를 가로채서 원하는 변환/주입을 할 수 있음.

                if (pool_ == nullptr) pool_ = &globalPool();
                // 아직 pool이 지정되지 않았다면 전역 풀을 디폴트로 주입(개념적 예제).
                // 실제 설계에서는 “부모 코루틴의 pool을 자식 Task에 전달”하는 식으로 발전 가능.

                std::cout << "[await_transform] pool id=" << pool_->id << "\n";
                return t;
                // 변환 결과로 Task<U>를 그대로 반환.
                // 여기에서 t 내부에 pool을 설정하거나, 래핑된 awaitable로 바꿀 수도 있음.
            }
        };

        using handle_type = std::coroutine_handle<promise_type>;
        // promise_type을 가진 coroutine handle 타입 별칭.

        handle_type h = nullptr;
        // 이 Task가 가리키는 코루틴 핸들(코루틴 프레임 포인터). 기본은 null.

        Task() = default;
        // 기본 생성자: 빈 Task(핸들 없음).

        explicit Task(handle_type hh) : h(hh) {}
        // 코루틴 핸들을 받아 Task로 감싸는 생성자.

        Task(Task&& o) noexcept : h(o.h) { o.h = nullptr; }
        // 이동 생성자: 핸들 소유권을 옮김(이동 후 원본은 null).

        Task(const Task&) = delete;
        // 복사 금지: 코루틴 핸들은 “소유권” 성격이 강하므로 중복 destroy 방지를 위해 copy 금지.

        ~Task() {
            if (h) h.destroy();
            // 소멸 시 코루틴 프레임 파괴.
            // ※ final_suspend가 suspend_never라면 코루틴이 끝난 시점에 이미 파괴되었을 수도 있어
            //    이런 구조는 수명 관리가 매우 위험해질 수 있음(실전에서는 보통 final_suspend를 suspend_always로 두고 외부가 destroy).
        }

        // 아주 단순 awaiter
        auto operator co_await() {
            // Task를 co_await 할 때 사용할 awaiter를 제공.
            // (operator co_await가 있으면 Task 자체가 awaitable이 됨)

            struct Awaiter {
                handle_type h; // 기다릴 대상 코루틴 핸들(여기선 사실상 사용 거의 안 함)

                bool await_ready() noexcept { return true; } // true => co_await 시 suspend 없이 즉시 진행
                // 즉, 이 awaiter는 “항상 완료된 것처럼” 동작한다(데모/더미 구현).

                void await_suspend(std::coroutine_handle<>) noexcept {}
                // await_ready가 true면 호출되지 않음.
                // 보통 여기서 기다릴 코루틴에 continuation을 등록하거나 스케줄링 한다.

                void await_resume() noexcept {}
                // await가 끝난 뒤 호출. 반환값/예외 전달에 사용.
            };

            return Awaiter{ h };
            // awaiter를 생성해서 반환.
        }
    };

    Task<void> child_async() {
        std::cout << "child_async running\n";
        co_return;
    }

    Task<void> parent_async(Pool& pool) {
        // ✅ “pool이 들어왔다고 가정” -> 실제로는 promise_type에 넣어야 하지만,
        // await_transform()이 출력만 해줌.
        std::cout << "parent_async start (pool.id=" << pool.id << ")\n";

        co_await child_async(); // await_transform(Task<U>)가 개입 가능

        std::cout << "parent_async end\n";
        co_return;
    }

    void await_transform_with_custom_type_use() {
        Pool myPool{ 123 };

        std::cout << "=== call parent_async ===\n";
        auto t = parent_async(myPool);

        // initial_suspend = suspend_never라서 이미 실행됨.

        std::cout << "=== end ===\n";
    }

    //=============================================================================================

    void Test()
    {
        await_transform_with_custom_type_use();

        await_transform_with_primitive_type_use();

        co_await_use();
        
        co_return_use();

        co_yield_use();

        coroutine_to_non_awaiter_use();

        coroutine_to_awaiter_use();

        Coroutine_what();
    }
}//Coroutine