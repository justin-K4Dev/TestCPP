#include "stdafx.h"

#include <format>
#include <vector>
#include <compare>



namespace ThreeWayCompare
{
    struct Point {
        int x, y;

        // 자동 비교 연산 지원 (멤버별로 <=>)
        auto operator<=>(const Point&) const = default;
    };

    void ThreeWay_Comparison()
    {
        /*
            📚 Three-way Comparison

              - C++20에서 도입된 통합 비교 연산자: <=> (Spaceship Operator, 우주선 연산자)
              - 객체 간의 <, <=, >, >=, ==, != 비교 연산을 자동으로 생성해주는 문법
              - 비교의 결과를 std::strong_ordering, std::weak_ordering, std::partial_ordering 등으로 표현
                * strong_ordering: 전형적 정렬(대소/동등/불일치 없음, ex: int, std::string)
                * weak_ordering: 동치(equivalent) 개념, 값이 같아도 내부적으로 다를 수 있음
                * partial_ordering: 비교 불가(NaN 등), 부동소수점에 적합
              - 직접 <=>만 구현하면, 나머지 비교 연산(<, == 등)은 자동 생성됨
              - 사용자 정의 타입의 비교 연산 오버로딩을 매우 간편하게 해줌
              - <compare> 헤더 필요, C++20 이상에서 지원
        */

        Point a{ 1, 2 };
        Point b{ 1, 3 };
        Point c{ 1, 2 };

        // <=> 연산자 결과값: strong_ordering
        auto res = (a <=> b);
        if (res < 0) std::cout << "a < b" << std::endl;
        if (res > 0) std::cout << "a > b" << std::endl;
        if (res == 0) std::cout << "a == b" << std::endl;

        // 아래와 같이 직관적인 비교 연산 가능(자동 생성됨)
        std::cout << std::boolalpha;
        std::cout << (a < b) << std::endl;   // true
        std::cout << (a == c) << std::endl;  // true
        std::cout << (b > c) << std::endl;   // true
    }


    void Test()
    {
        ThreeWay_Comparison();
    }
}