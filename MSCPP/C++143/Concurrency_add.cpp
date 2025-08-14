#include "stdafx.h"

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <barrier>
#include <latch>
#include <semaphore>
#include <thread>


namespace Concurrency_AddFeatures
{
    void std_barrier_use()
    {
        {
            std::barrier sync_point(3); // 스레드 3개가 도달하면 unblock

            auto worker = [&](int id) {
                std::cout << "[thread " << id << "] 준비됨\n";
                sync_point.arrive_and_wait(); // barrier 도달 대기
                std::cout << "[thread " << id << "] 동시에 시작\n";
            };

            std::thread t1(worker, 1);
            std::thread t2(worker, 2);
            std::thread t3(worker, 3);

            t1.join(); t2.join(); t3.join();
        }

        system("pause");
    }

    //=============================================================================================

    void std_latch_use()
    {
        {
            std::latch ready(3); // count 3

            auto loader = [&](int id) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
                std::cout << "[thread " << id << "] 작업 완료\n";
                ready.count_down();
            };

            std::thread t1(loader, 1);
            std::thread t2(loader, 2);
            std::thread t3(loader, 3);

            ready.wait(); // 모든 count_down 전까지 대기
            std::cout << "[main] 모든 스레드 준비 완료\n";

            t1.join(); t2.join(); t3.join();
        }

        system("pause");
    }

    //=============================================================================================

    std::counting_semaphore<2> sem(2); // 최대 동시 2개 허용
    void semaphore_worker(int id) {
        sem.acquire(); // 리소스 획득
        std::cout << "[worker " << id << "] 시작\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "[worker " << id << "] 종료\n";
        sem.release(); // 리소스 반환
    }

    void std_counting_semaphore_use()
    {
        {
            std::vector<std::thread> workers;

            // 총 5개의 작업자 스레드 시작
            for (int i = 0; i < 5; ++i) {
                workers.emplace_back(semaphore_worker, i + 1);
            }

            // 스레드 조인
            for (auto& t : workers) t.join();

            std::cout << "[main] 모든 작업 완료\n";
        }

        system("pause");
    }

    //=============================================================================================

    void Concurrency_AddFeatures()
    {
        /*
            📚 동시성 추가 기능(C++20)

              - C++20에서는 동기화 객체와 컨테이너 편의 함수들이 대거 추가되어
                멀티스레드 프로그래밍 및 자료구조 탐색이 더 간결하고 안전하게 개선됨

              🔹 동기화 관련 새 기능 (헤더 <barrier>, <latch>, <semaphore>)

                1. ✅ std::barrier
                  - 지정된 스레드 수가 도달할 때까지 대기 후 일제히 재개
                  - 여러 라운드 지원, reusable (멀티패스)

                2. ✅ std::latch
                  - 지정된 카운트가 0이 될 때까지 스레드가 대기
                  - 일회성, 단순 동기화용

                3. ✅ std::counting_semaphore / binary_semaphore
                  - 세마포어: 리소스 카운팅 및 제어
                  - producer-consumer, 큐 동기화 등에 적합

              🔹 도입 배경
                - 동기화 객체는 Boost 등 외부 라이브러리에 의존했으나 이제 표준 지원

              🔹 예제 문법
                std::barrier sync(3);           // 3개 스레드 도달 시 재개
        */
        {
            std_barrier_use();

            std_latch_use();

            std_counting_semaphore_use();
        }

        system("pause");
    }


    void Test()
    {
        Concurrency_AddFeatures();
    }
}//Concurrency_AddFeatures