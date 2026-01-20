#include "stdafx.h"

#include <windows.h>

#include <thread>

namespace ThreadSyncWithInterLock
{

    // Interlocked 테스트
    void thread_sync_with_interlock()
    {
        /*
            📚 std::thread + Interlocked 동기화 패턴 정리 (Win32)

              1. Interlocked
                - 여러 스레드가 같은 메모리(4/8바이트)를 건드릴 때,
                  "읽고-계산하고-다시 쓰기"를 CPU 단위로 원자적으로 해주는 Win32 함수들.
                - 예) InterlockedIncrement, InterlockedDecrement, InterlockedExchange, InterlockedCompareExchange ...
                - 커널 오브젝트(이벤트, 뮤텍스)까지는 쓰기 싫고, 아주 가벼운 스핀/카운터만 하고 싶을 때 사용.

              2. 왜 쓰냐?
                - std::mutex보다 훨씬 가볍다. (커널 모드 안 들어감)
                - 단점: 긴 임계구역에는 부적합. 바쁜 대기(spin) 구간이 길면 CPU만 태움.

              3. 여기서 보여줄 예제
                (1) 여러 스레드가 하나의 카운터를 InterlockedIncrement로 올리는 경우
                (2) InterlockedCompareExchange로 아주 단순한 스핀락 구현
                (3) InterlockedExchange를 이용한 "한 번만 실행" 플래그
                (4) 위를 std::thread 여러 개로 테스트

           
              4. 테스트 예제
                (1) 카운터 누적 예제
                  - 공유 변수: volatile long g_counter;
                  - 여러 스레드가 동시에 증가 → 레이스 컨디션 없이 정확한 값 나와야 함.

                (2) 단순 스핀락 예제
                  - long lock = 0;
                  - lock이 0이면 1로 바꾸고 들어감
                    while (InterlockedCompareExchange(&lock, 1, 0) != 0) {
                        // spin
                    }
                    ... 임계구역 ...
                    InterlockedExchange(&lock, 0);
                  - 실제 서비스에서는 _mm_pause() 같은 거 넣어서 과열을 막는 게 좋다.

                (3) one - time init 예제
                  - long inited = 0;
                    if (InterlockedExchange(&inited, 1) == 0) {
                        // 처음 들어온 스레드
                    }
                    else {
                        // 이미 누가 진입
                    }
                        - std::call_once 비슷한 아주 가벼운 버전
        */

         // (1) 공유 카운터 테스트
        {
            std::cout << "\n[TEST 1] InterlockedIncrement counter\n";

            volatile long g_counter = 0;
            const int threadCount = 8;
            const int perThread = 50'000;

            auto worker = [&g_counter, perThread]() {
                for (int i = 0; i < perThread; ++i) {
                    // 원자 증가
                    InterlockedIncrement(&g_counter);
                }
            };

            std::vector<std::thread> threads;
            threads.reserve(threadCount);
            for (int i = 0; i < threadCount; ++i)
                threads.emplace_back(worker);

            for (auto& t : threads) t.join();

            // 기대값: threadCount * perThread
            std::cout << "  expected: " << (threadCount * perThread) << "\n";
            std::cout << "  actual  : " << g_counter << "\n";
        }

        // (2) 아주 간단한 스핀락 구현 + 공유 벡터 push_back 보호
        {
            std::cout << "\n[TEST 2] SpinLock with InterlockedCompareExchange\n";

            // 0 = unlocked, 1 = locked
            volatile long spin = 0;
            std::vector<int> shared;
            const int threadCount = 4;
            const int perThread = 10'000;

            auto lock = [&spin]() {
                // 0 → 1로 바꾸는 데 성공할 때까지 스핀
                while (InterlockedCompareExchange(&spin, 1, 0) != 0) {
                    // 바쁜 대기: 너무 빡세면 여기서 Sleep(0)이나 _mm_pause() 넣기
                }
            };
            auto unlock = [&spin]() {
                InterlockedExchange(&spin, 0);
            };

            auto worker = [&]() {
                for (int i = 0; i < perThread; ++i) {
                    lock();
                    shared.push_back(i);
                    unlock();
                }
            };

            std::vector<std::thread> threads;
            for (int i = 0; i < threadCount; ++i)
                threads.emplace_back(worker);
            for (auto& t : threads) t.join();

            std::cout << "  expected size: " << (threadCount * perThread) << "\n";
            std::cout << "  actual size  : " << shared.size() << "\n";
            std::cout << "  (스핀이라 CPU 점유율이 높을 수 있음)\n";
        }

        // (3) 한 번만 실행(one-time init) 패턴
        {
            std::cout << "\n[TEST 3] one-time init with InterlockedExchange\n";

            volatile long inited = 0;
            const int threadCount = 6;

            auto worker = [&]() {
                // 0이었으면 1로 변경 → 내가 첫 번째
                if (InterlockedExchange(&inited, 1) == 0) {
                    std::cout << "  init by thread " << std::this_thread::get_id() << "\n";
                    // 초기화 작업...
                }
                else {
                    // 이미 누가 했음
                    // std::cout << "  skip by thread " << std::this_thread::get_id() << "\n";
                }
            };

            std::vector<std::thread> threads;
            for (int i = 0; i < threadCount; ++i)
                threads.emplace_back(worker);
            for (auto& t : threads) t.join();
        }

        // (4) Interlocked + busy flag 로 아주 단순한 producer-like 패턴
        //     (실제 프로덕션에서는 condition_variable 이나 이벤트를 쓰는 게 맞음)
        {
            std::cout << "\n[TEST 4] busy flag sample\n";

            volatile long busy = 0;   // 0: idle, 1: busy
            int shared_value = 0;

            auto producer = [&]() {
                for (int i = 0; i < 5; ++i) {
                    // busy 플래그를 0→1로 바꾸면서 점유
                    while (InterlockedCompareExchange(&busy, 1, 0) != 0) {
                        // 대기
                    }
                    shared_value = i;
                    std::cout << "  producer set " << i << "\n";
                    // 풀어줌
                    InterlockedExchange(&busy, 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            };

            auto consumer = [&]() {
                int last = -1;
                int count = 0;
                while (count < 5) {
                    // 점유 시도
                    if (InterlockedCompareExchange(&busy, 1, 0) == 0) {
                        if (shared_value != last) {
                            std::cout << "  consumer got " << shared_value << "\n";
                            last = shared_value;
                            ++count;
                        }
                        // 해제
                        InterlockedExchange(&busy, 0);
                    }
                    // 너무 타이트하지 않게
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            };

            std::thread pt(producer);
            std::thread ct(consumer);
            pt.join();
            ct.join();
        }

        std::cout << "\n[Press any key...]\n";
        system("pause");
    }

    void Test()
    {
        thread_sync_with_interlock();
    }
}