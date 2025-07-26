#include "stdafx.h"

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <bitset>


namespace Atomic_AddFeatures
{
    void Atomic_AddFeatures()
    {
        /*
            📚 <atomic> 개선 (C++20)

              - C++20에서는 기존 원자 타입 지원에 더해 **참조 기반 원자성**, **배열 원자 타입**, **비트 단위 원자 연산** 등을 추가함
              - 멀티스레드 프로그래밍에서 더 정밀하고 유연한 동기화를 구현할 수 있도록 설계됨

              🔹 주요 추가 기능

                1. ✅ std::atomic_ref<T>
                  - 기존 객체(변수)에 **원자성을 부여하는 참조 타입**
                  - 공유 메모리 접근, 캐시 일관성 확보 등에 활용
                  - 기존 변수에 대해 atomic 연산 수행 가능

                  예:
                    int x = 0;
                    std::atomic_ref<int> ax(x);
                    ax.fetch_add(1);

                2. ✅ std::atomic<T[]> (atomic array)
                  - 원자 배열 타입 지원
                  - 배열의 각 요소에 대해 독립적인 atomic 연산 수행 가능
                  - 동적 크기의 배열도 new/delete와 함께 사용 가능

                  예:
                    std::atomic<int> arr[10];
                    arr[0].store(42);

                3. ✅ 비트 단위 연산 함수 (atomic_flag 개선)
                  - `std::atomic<T>`에 대해 `fetch_or`, `fetch_and`, `fetch_xor` 등 비트 연산이 지원됨
                  - 스핀락, 플래그, 권한 비트마스크 등의 처리에 유용

                  예:
                    flags.fetch_or(0b0100);  // 특정 비트 ON

              🔹 도입 배경
                - 기존 atomic<T>는 복사/참조가 불가능했음
                - 여러 스레드가 동일 객체에 접근하는 경우, 참조 원자성이 요구됨
                - 구조체, 배열, 비트마스크 수준의 동기화 처리 수요 반영

              🔹 표준 지원
                - C++20 이상
                - Visual Studio 2019 16.10+, GCC 10+, Clang 11+ 이상 대부분 지원

              🔹 주의 사항
                - atomic_ref는 **정렬 요건(alignment)**을 만족해야 하며, 정의 시 반드시 유의할 것
                - atomic<T[]>는 배열 요소마다 atomic을 보장하며, **std::atomic<int[N]> 은 아님**

              🔹 예제 문법

                std::atomic_ref<int> aref(x);
                aref.fetch_add(1);

                std::atomic<int> arr[4];
                arr[2].fetch_or(0b0010);
        */
        {
            using namespace std;

            // ✅ std::atomic_ref 예제
            {
                int raw = 0;
                atomic_ref<int> aref(raw);

                vector<thread> threads;
                for (int i = 0; i < 4; ++i) {
                    threads.emplace_back([&]() {
                        for (int j = 0; j < 1000; ++j)
                            aref.fetch_add(1);
                    });
                }
                for (auto& t : threads) t.join();

                cout << "[atomic_ref] raw = " << raw << "\n"; // 기대값: 4000
            }

            // ✅ atomic<T[]> 배열 예제
            {
                constexpr int N = 4;
                atomic<int> arr[N];
                for (int i = 0; i < N; ++i) arr[i].store(i * 10);

                arr[2].fetch_add(5);
                cout << "[atomic<T[]>] arr[2] = " << arr[2] << "\n"; // 기대값: 25
            }

            // ✅ 비트 연산 (fetch_or)
            {
                atomic<int> flags = 0b0101;
                flags.fetch_or(0b0010); // 0b0111
                cout << "[bitwise fetch_or] flags = " << bitset<4>(flags.load()) << "\n"; // 0111
            }
        }

        system("pause");
    }


    void Test()
    {
        Atomic_AddFeatures();
    }
}//Atomic_AddFeatures