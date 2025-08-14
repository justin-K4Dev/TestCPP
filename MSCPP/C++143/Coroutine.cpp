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
                    - 코루틴의 상태를 관리
                    - 결과 값 또는 예외 처리, suspend/return 동작 정의

                2. coroutine_handle<>:
                    - 코루틴 객체를 제어 (resume, destroy 등)

                3. awaiter / awaitable:
                    - co_await 대상 객체는 이 인터페이스를 따라야 함
                    - 함수: await_ready(), await_suspend(), await_resume()

              🔹 반환 타입
                - 일반 함수와 달리 Coroutine은 특별한 반환 타입 필요
                - 예: `Generator<T>`, `Task<T>`, `Lazy<T>` 등 (사용자가 직접 정의하거나 라이브러리 사용)

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
        */
    }

    //=============================================================================================

    struct IntGenerator 
    {
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type 
        {
            int current_value;
            std::suspend_always yield_value(int value) 
            {
                current_value = value;
                return {};
            }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            IntGenerator get_return_object() 
            {
                return IntGenerator{ handle_type::from_promise(*this) };
            }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };

        handle_type handle;
        explicit IntGenerator(handle_type h) : handle(h) {}
        IntGenerator(const IntGenerator&) = delete;
        IntGenerator(IntGenerator&& other) noexcept : handle(other.handle) { other.handle = nullptr; }
        ~IntGenerator() { if (handle) handle.destroy(); }

        // iterator-like 인터페이스 제공
        bool next() 
        {
            if (!handle.done()) handle.resume();
            return !handle.done();
        }

        int value() const { return handle.promise().current_value; }
    };

    /* 실제 코루틴 함수 */
    IntGenerator counter(int from, int to) 
    {
        for (int i = from; i <= to; ++i) 
        {
            co_yield i; // 중간 값을 호출자에게 전달하고 중단(suspend)
        }
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
        */
        {
            auto gen = counter(1, 3);
            while (gen.next()) {
                std::cout << gen.value() << " ";
            }
            std::cout << std::endl; // 출력: 1 2 3

            system("pause");
        }
    }

    //=============================================================================================

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

    VoidTask co_return_use() 
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
            std::cout << "[co_return_use()] 작업 시작\n";

            // 예시 작업: 루프 처리
            for (int i = 1; i <= 3; ++i) {
                std::cout << "  - 작업 #" << i << " 처리 중...\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 모의 대기
            }

            std::cout << "[co_return_use()] 작업 완료\n";
            co_return; // void 반환
        }
    }

    //=============================================================================================

    struct SimpleAwaiter 
    {
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<>) const noexcept { std::cout << "[suspending...]\n"; }
        void await_resume() const noexcept { std::cout << "[resumed!]\n"; }
    };

    struct AwaitTask 
    {
        struct promise_type 
        {
            AwaitTask get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() {}
        };
    };

    AwaitTask co_await_use()
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
        {
            co_await SimpleAwaiter{};
            std::cout << "work done\n";

            system("pause");
        }
    }

    //=============================================================================================

    void Test()
    {
        co_await_use();

        co_return_use();

        co_yield_use();

        Coroutine_what();
    }
}//Coroutine