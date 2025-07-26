#include "stdafx.h"

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <barrier>
#include <latch>
#include <semaphore>
#include <thread>


namespace Container_AddFeatures
{
    void std_unordered_map_use()
    {
        {
            std::unordered_map<std::string, int> table = {
                {"apple", 1}, {"banana", 2}
            };

            if (table.contains("apple"))
                std::cout << "[map] apple 있음\n";

            std::map<int, std::string> ordered = { {1, "one"}, {2, "two"} };
            if (ordered.contains(2))
                std::cout << "[ordered map] 2 있음\n";

            std::vector<int> v = { 10, 20, 30 };
            std::cout << "[vector.at(1)] = " << v.at(1) << "\n"; // 안전 접근
        }

        system("pause");
    }

    void Container_AddFeatures()
    {
        /*
            📚 컨테이너 추가 기능 (C++20)

              - C++20에서는 컨테이너 편의 함수들이 대거 추가되어
                **멀티스레드 프로그래밍 및 자료구조 탐색이 더 간결하고 안전하게 개선**됨

              🔹 컨테이너 메서드 강화 (C++20)

                - `std::map::contains(key)`               → 키 존재 여부 확인
                - `std::unordered_map::contains(key)`     → 해시 탐색 개선
                - `std::vector::contains(value)` (C++23)  → 순차 컨테이너 확장 (🔺주의: C++23)
                - `std::vector::at(i)`                    → 범위 검사 포함된 안전한 접근 (C++11부터)

              🔹 도입 배경
                - 기존 STL에서는 존재 여부 확인 시 `.find(...) != .end()` 패턴 필요 → 번거로움

              🔹 예제 문법
                std::unordered_map m; m.contains("key");
                std::vector v = {1, 2, 3}; v.at(1);
        */
        {
            std_unordered_map_use();
        }

        system("pause");
    }


    void Test()
    {
        Container_AddFeatures();
    }
}//Container_AddFeatures