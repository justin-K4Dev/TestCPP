#include "stdafx.h"

#include <windows.h>

#include <thread>

namespace ThreadSyncWithSRWLock
{
    void thread_sync_with_SRWLock()
    {
        /*
            📚 SRWLock (Slim Reader/Writer Lock)

              - Windows 전용 경량 동기화 객체.
              - Slim Reader/Writer Lock = 여러 reader 동시 허용, writer는 단독.
              - 커널 오브젝트(handle) 안 써서 가볍다.
              - 초기화도 그냥 SRWLOCK lock = SRWLOCK_INIT; 로 끝.
              - 함수:
                AcquireSRWLockExclusive / ReleaseSRWLockExclusive
                AcquireSRWLockShared    / ReleaseSRWLockShared
              - 주의: 재귀락 아님. 같은 스레드가 같은 SRWLock을 다시 잡으면 막힌다.


            ✅ SRWLock Read/Write 진입 가능 여부

              | 현재 락 상태 (이미 잡힘) | 새로 들어오려는 Read(Shared)    | 새로 들어오려는 Write(Exclusive)  | 설명                                                       
              |--------------------------|---------------------------------|-----------------------------------|-------------------------------------------------------------
              | 없음                     | 가능                            | 가능                              | 아무도 안 잡았으니 둘 다 들어올 수 있음                    
              | 1개 이상 Read            | 가능                            | 불가 (대기)                       | 여러 리더는 동시 허용, 하지만 그 사이에 라이터는 못 들어옴 
              | Write 1개                | 불가 (대기)                     | 불가 (대기)                       | 독점 중이므로 다른 읽기/쓰기 전부 대기             
              | Write가 끝나서 해제됨    | 대기 중이던 것 중에서 OS가 선택 | 대기 중이던 것 중에서 OS가 선택   | 해제되는 순간부터 다시 규칙 적용                 


            ◈ 아래 예제:
              (1) 여러 reader가 동시에 읽는지 확인
              (2) writer가 들어오면 단독으로 들어가는지 확인
              (3) std::thread 여러 개로 간단 부하 테스트
        */

        // 공유 자원
        SRWLOCK rwlock = SRWLOCK_INIT;
        int shared_value = 0;

        // (1) 여러 reader ------------------------------------------
        auto reader = [&](int id) {
            for (int i = 0; i < 5; ++i) {
                AcquireSRWLockShared(&rwlock);
                // 여러 reader가 동시에 여기 들어올 수 있어야 함
                std::cout << "[R" << id << "] read=" << shared_value << "\n";
                ReleaseSRWLockShared(&rwlock);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };

        // (2) writer ------------------------------------------------
        auto writer = [&]() {
            for (int i = 1; i <= 5; ++i) {
                AcquireSRWLockExclusive(&rwlock);
                shared_value = i;
                std::cout << "   [W] write=" << shared_value << "\n";
                ReleaseSRWLockExclusive(&rwlock);
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        };

        std::cout << "\n[SRWLock TEST]\n";

        // reader 여러 개 + writer 1개
        std::vector<std::thread> threads;
        threads.emplace_back(writer);
        threads.emplace_back(reader, 1);
        threads.emplace_back(reader, 2);
        threads.emplace_back(reader, 3);

        for (auto& t : threads) t.join();

        // (3) 부하 테스트: reader 위주
        {
            std::cout << "\n[SRWLock read-heavy TEST]\n";

            SRWLOCK rw = SRWLOCK_INIT;
            int value = 0;
            const int threadCount = 6;

            auto heavy_reader = [&]() {
                for (int i = 0; i < 10000; ++i) {
                    AcquireSRWLockShared(&rw);
                    int v = value; // 읽기만
                    (void)v;
                    ReleaseSRWLockShared(&rw);
                }
            };

            auto heavy_writer = [&]() {
                for (int i = 0; i < 1000; ++i) {
                    AcquireSRWLockExclusive(&rw);
                    value++;
                    ReleaseSRWLockExclusive(&rw);
                }
            };

            std::vector<std::thread> th;
            // reader 다수
            for (int i = 0; i < threadCount - 1; ++i)
                th.emplace_back(heavy_reader);
            // writer 1개
            th.emplace_back(heavy_writer);

            for (auto& t : th) t.join();

            std::cout << "final value = " << value << "\n";
        }

        std::cout << "\n(done)\n";
        system("pause");
    }

    void Test()
    {
        thread_sync_with_SRWLock();
    }
}