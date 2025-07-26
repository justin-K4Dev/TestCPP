#include "stdafx.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stop_token>


namespace Thread_AddFeature
{
    void Thread_AddFeature()
    {
        /*
            📚 <thread> 개선 (C++20)

              - C++20에서는 기존 `std::thread`의 한계를 보완하기 위해 **joinable thread**, **취소 가능한 thread** 개념이 도입됨
              - `std::jthread`, `std::stop_token`, `std::stop_source`가 핵심 구성 요소

              🔹 std::jthread
                - `std::thread`의 개선 버전
                - 생성시 자동 실행되고, **소멸시 자동 join()** 수행 → 리소스 누수 방지
                - `std::stop_token` 을 통해 외부에서 **정지 요청(stop request)** 가능
                - 기본적으로 RAII 기반 안전 스레드로 설계됨

              🔹 std::stop_token / stop_source
                - `stop_source`: 스레드 종료 요청을 트리거하는 객체
                - `stop_token`: 스레드 내부에서 취소 여부를 체크하는 객체
                - cooperative cancellation (협조적 종료) 패턴 구현에 적합

              🔹 주요 장점
                - 스레드 종료 누락 방지 (join 자동 호출)
                - 취소 가능한 구조 내장 (stop 요청 전파)
                - 동기화 없이 안전하게 스레드 종료 감지 가능
                - poll 또는 blocking wait 기반 long-running 작업에 적합

              🔹 표준 지원
                - C++20 이상
                - Visual Studio 2019 16.10+, GCC 11+, Clang 14+ 이상에서 대부분 지원

              🔹 예제 문법

                std::jthread worker([](std::stop_token st) {
                    while (!st.stop_requested()) { ... }
                });

                // 외부에서 취소 요청
                worker.request_stop();
        */
        {
            using namespace std;
            using namespace chrono;

            // 자동 실행 + 자동 join + 취소 토큰 수신
            std::jthread worker([](std::stop_token stoken) {
                int count = 0;
                while (!stoken.stop_requested()) {
                    cout << "[worker] count: " << count++ << "\n";
                    this_thread::sleep_for(500ms);
                }
                cout << "[worker] 종료 요청 수신됨\n";
            });

            // 메인 스레드에서 2초 후 취소 요청
            this_thread::sleep_for(2s);
            cout << "[main] 작업 취소 요청\n";
            worker.request_stop(); // stop_token → stop_requested() == true

            // worker는 소멸 시 자동 join() 됨

            /*
            출력:
                [worker] count: 0
                [worker] count: 1
                [worker] count: 2
                [worker] count: 3
                [main] 작업 취소 요청
                [worker] 종료 요청 수신됨
            */ 
        }

        system("pause");
    }

    //=============================================================================================

    void async_wait_with_stop(std::stop_token stoken)
    {
        using namespace std;
        using namespace std::chrono;

        mutex mtx;
        condition_variable_any cv;
        bool ready = false;

        // ✅ stop_callback 등록: stop_token 이 stop_requested 되면 즉시 cv.notify_all()
        std::stop_callback callback(stoken, [&]() {
            cout << "[callback] stop 요청 수신 → 조건 변수 깨움\n";
            cv.notify_all();
        });

        unique_lock lock(mtx);
        cout << "[worker] 조건 변수 대기 중...\n";

        // stop_token 과 함께 조건 변수 대기
        while (!stoken.stop_requested() && !ready)
        {
            // timeout 없이 무한 대기 (stop_token이 요청되면 callback 통해 깨움)
            cv.wait(lock);
        }

        if (stoken.stop_requested())
            cout << "[worker] stop_token 에 의해 중단됨\n";
        else
            cout << "[worker] ready 조건 충족\n";
    }

    void cancelable_thread_wait()
    {
        /*
            📚 std::stop_token + stop_callback + condition_variable_any (C++20)

              🔹 stop_token
                - `std::jthread`와 함께 전달되어, 스레드 내부에서 **중단 요청 여부를 확인**할 수 있는 객체

              🔹 stop_callback
                - `stop_token`이 중단 요청을 수신했을 때 **즉시 실행할 콜백 함수**를 등록할 수 있음
                - 조건 변수, 리소스 클린업, 타임아웃 처리 등에 활용됨

              🔹 condition_variable_any
                - `std::mutex` 외의 모든 Lockable 객체를 지원하는 유연한 조건 변수
                - `std::unique_lock<std::mutex>` 외에 `std::timed_mutex`, `std::shared_mutex` 등도 사용 가능
                - `stop_token`과 함께 중단 가능 `wait()`을 구현할 수 있음

              🔹 활용 목적
                - 무한 대기 중인 스레드를 외부에서 **즉시 깨우기(stop)** 위한 방법
                - 조건 변수 기반 기다림을 안전하게 종료
        */
        {
            using namespace std;
            using namespace std::chrono;

            std::jthread th(async_wait_with_stop); // stop_token 자동 전달
            this_thread::sleep_for(2s);

            cout << "[main] 2초 후 stop 요청\n";
            th.request_stop(); // stop_callback 트리거 → cv 깨움 → 스레드 종료
        }

        system("pause");
    }


    void Test()
    {
        cancelable_thread_wait();

        Thread_AddFeature();
    }
}//Thread_AddFeature