#include "stdafx.h"


#include <iostream>
#include <coroutine>   // For co_yield (C++20)


namespace VCPP_Improvements
{
    // 1. Coroutine: Use co_yield (C++20)
    struct Counter {
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;
        struct promise_type {
            int value;
            Counter get_return_object() { return Counter{ handle_type::from_promise(*this) }; }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            std::suspend_always yield_value(int v) { value = v; return {}; }
            void return_void() {}
            void unhandled_exception() { std::terminate(); }
        };
        handle_type handle;
        explicit Counter(handle_type h) : handle(h) {}
        ~Counter() { if (handle) handle.destroy(); }
        bool next() { handle.resume(); return !handle.done(); }
        int value() const { return handle.promise().value; }
    };

    Counter simple_counter(int n) {
        for (int i = 0; i < n; ++i)
            co_yield i; // co_yield replaces experimental yield
    }

    void coroutine()
    {
        std::cout << "[Coroutine] ";
        auto c = simple_counter(3);
        while (c.next()) {
            std::cout << c.value() << " ";
        }
        std::cout << std::endl;

        /*
            출력:
            [Coroutine] 0 1 2
        */
    }

    void VCPP_Improvements()
    {
        /*
            📚 Visual C++ 최신 릴리즈 주요 개선사항

              1. 코루틴(Coroutine) 관련 변화:
                • 기존 실험적 키워드 "yield"(/await 스위치) 제거, C++ 표준의 "co_yield"로 통일
                • 코루틴 사용 코드는 co_yield로 업데이트 필요

              - 참고: Visual C++ Compiler Version, C++ compiler diagnostics improvements, Control Flow Guard, Visual C++ Team Blog 등
        */

        coroutine();
    }

    void Test()
    {
        //VCPP_Improvements();
    }

}//VCPP_Improvements