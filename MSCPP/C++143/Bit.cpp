#include "stdafx.h"

#include <iostream>
#include <bit>
#include <bitset>



namespace Bit
{
    void Bit_what()
    {
        /*
            📚 <bit> 추가 (C++20)

              - C++20에서 새롭게 도입된 로우레벨 비트 연산

              🔹 특징
                - 정수 기반 비트 연산 최적화 함수 제공
                - 하드웨어 인스트럭션과 매핑되는 함수들이 많아 고성능 처리 가능
                - 대부분 `constexpr`로 제공되어 컴파일 타임 연산 최적화 가능

              🔹 주요 함수
                - `std::has_single_bit(n)`         → n이 2의 거듭제곱인지 확인
                - `std::bit_ceil(n)`               → n 이상인 가장 작은 2의 거듭제곱
                - `std::bit_floor(n)`              → n 이하인 가장 큰 2의 거듭제곱
                - `std::bit_width(n)`              → n을 표현하는 데 필요한 비트 수
                - `std::rotl(x, s)`, `std::rotr(x, s)` → 비트 rotate (left/right)
                - `std::countl_zero(n)`, `std::countr_one(n)` → 비트 앞/뒤에서 0/1 개수 카운트
        */
        {
            std::cout << std::boolalpha;

            // <bit> 테스트
            unsigned int x = 13;

            std::cout << "[bit test]\n";
            std::cout << "has_single_bit(8): " << std::has_single_bit(8u) << "\n";   // true
            std::cout << "bit_ceil(13): " << std::bit_ceil(x) << "\n";              // 16
            std::cout << "bit_floor(13): " << std::bit_floor(x) << "\n";            // 8
            std::cout << "bit_width(13): " << std::bit_width(x) << "\n";            // 4 (1101)
            std::cout << "countl_zero(13): " << std::countl_zero(x) << "\n";        // 플랫폼에 따라 다름
            std::cout << "rotr(0b1101, 1): " << std::bitset<4>(std::rotr(0b1101u, 1)) << "\n"; // 1011
            std::cout << "rotl(0b1101, 1): " << std::bitset<4>(std::rotl(0b1101u, 1)) << "\n"; // 1010
        }

        system("pause");
    }


    void Test()
    {
        Bit_what();
    }
}//Bit