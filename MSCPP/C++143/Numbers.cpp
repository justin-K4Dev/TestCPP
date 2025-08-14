#include "stdafx.h"


#include <iostream>
#include <numbers>
#include <cmath>


namespace Numbers
{

    void Numbers_what()
    {
        /*
            📚 <numbers> 추가 (C++20)

              - C++20에서 새롭게 도입된 고정 상수 제공 헤더

              🔹 특징
                - 고정된 수학 상수를 `constexpr`로 제공
                - 코드에서 직접 `π`, `e`, `ln2` 등의 값을 정의할 필요 없음
                - 컴파일 타임 상수로 수학 코드의 정확도 및 표현력 향상

              🔹 주요 함수
                - `std::numbers::pi`       → 원주율 π
                - `std::numbers::e`        → 자연상수 e
                - `std::numbers::sqrt2`    → √2
                - `std::numbers::ln2`      → ln(2)
                - `std::numbers::log10e`   → log₁₀(e)
        */
        {
            std::cout << std::boolalpha;

            std::cout << "\n[numbers test]\n";
            std::cout << "pi       : " << std::numbers::pi << "\n";
            std::cout << "e        : " << std::numbers::e << "\n";
            std::cout << "sqrt2    : " << std::numbers::sqrt2 << "\n";
            std::cout << "ln2      : " << std::numbers::ln2 << "\n";
            std::cout << "log10e   : " << std::numbers::log10e << "\n";

            std::cout << "\n원 둘레(반지름 5): " << 2 * std::numbers::pi * 5 << "\n";
        }

        system("pause");
    }


    void Test()
    {
        Numbers_what();
    }
}