#include "stdafx.h"


#include <vector>
#include <iostream>


namespace GeneralizedRangeBasedForLoops
{
    // Custom iterator/sentinel example
    struct OddRange {
        struct iterator {
            int value;
            int operator*() const { return value; }
            iterator& operator++() { value += 2; return *this; }
            bool operator!=(const iterator& rhs) const { return value != rhs.value; }
        };
        struct sentinel {
            int stop;
            friend bool operator!=(const iterator& it, const sentinel& s) {
                return it.value < s.stop;
            }
        };
        iterator begin() const { return { 1 }; }
        sentinel end() const { return { 10 }; }
    };

	void RangeBasedForLoop()
	{
        /*
            📚 Generalized range-based for loops 개요 (C++20~)

              - 기존(C++11~17)의 range-based for 문은 반드시 begin()과 end()의 반환 타입이 동일해야 함
              - C++20부터는 begin()과 end()가 서로 다른 타입(특히 end가 sentinel 객체 등)이어도 동작함
                → Ranges 라이브러리, 사용자 정의 이터레이터, sentinel 기반 반복 등 지원
              - range-v3 같은 라이브러리에서 유연한 순회가 가능해짐
              - 표준 라이브러리 ranges, 커스텀 컨테이너, 파일/네트워크 스트림 등 확장성 대폭 증가
              - 별도의 컴파일러 스위치 없이 C++20부터 지원
              - 참고: https://github.com/ericniebler/range-v3
        */

        std::cout << "Odd numbers (custom iterator/sentinel): ";
        for (int v : OddRange()) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
        // 출력: Odd numbers (custom iterator/sentinel): 1 3 5 7 9

        // 기존 STL 컨테이너와 호환
        std::array<int, 3> arr{ 10, 20, 30 };
        for (int n : arr) {
            std::cout << "arr: " << n << std::endl;
        }
        /*
            출력:
                arr: 10
                arr: 20
                arr: 30
        */
	}

	void Test()
	{
        RangeBasedForLoop();
	}

}//end of namespace