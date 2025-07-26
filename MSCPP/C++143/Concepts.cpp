#include "stdafx.h"

#include <concepts>
#include <iostream>
#include <string>



namespace Concepts
{
    /// 기본 Concept 예시: 정수 타입만 허용
    template <std::integral T>
    T add(T a, T b) {
        return a + b;
    }

    // 사용자 정의 Concept: "Print 가능 타입"
    template <typename T>
    concept Printable = requires(T a) {
        { std::cout << a } -> std::same_as<std::ostream&>;
    };

    // Printable을 만족하는 함수
    template <Printable T>
    void print_twice(const T& value) {
        std::cout << value << " " << value << std::endl;
    }

    // 정수형 타입만 허용
    template <typename T>
    requires std::integral<T>
    void print_integer(T value) {
        std::cout << "정수: " << value << '\n';
    }

    // 멤버 함수에 requires 사용
    template <typename T>
    struct Printer {
        void print(T value) requires std::integral<T> {
            std::cout << "정수형: " << value << '\n';
        }

        void print(T value) requires std::floating_point<T> {
            std::cout << "실수형: " << value << '\n';
        }
    };

    // requires 절을 별도 표현식으로 사용
    template <typename T>
    void display(T val) requires (sizeof(T) <= 4)
    {
        std::cout << "4바이트 이하 타입\n";
    }

    template <typename T>
    void display(T val) requires (sizeof(T) > 4)
    {
        std::cout << "4바이트 초과 타입\n";
    }

    void Concepts()
    {
        /*
            📚 Concepts (C++20~)

              - Concepts는 C++20에서 도입된 **템플릿 타입 제약 조건**을 정의하는 새로운 언어 기능
              - 기존 템플릿은 타입/연산 요구사항을 컴파일 에러로만 알렸으나,
                Concepts를 사용하면 "이 템플릿 파라미터는 이런 조건을 만족해야 한다"를 명시적으로 선언 가능
              - 코드의 **가독성, 에러 메시지 명확화, 재사용성, 의도 표현력** 크게 향상
              - 표준 라이브러리에도 std::integral, std::floating_point, std::ranges::range 등
                다양한 기본 Concepts 제공
              - user-defined Concepts(사용자 정의)도 손쉽게 정의 가능
              - requires, concept 키워드로 작성
              - 함수, 클래스, 템플릿 등 다양한 위치에서 타입 제약에 활용
              - 헤더: <concepts>
        */
        {
            // add: int, long 등 정수 타입만 허용됨
            std::cout << add(3, 5) << std::endl;    // 출력: 8
            // std::cout << add(3.1, 5.2) << std::endl; // 컴파일 에러 (정수만 허용)

            print_twice(42);        // 출력: 42 42
            print_twice(std::string("hello")); // 출력: hello hello
            // print_twice(std::vector<int>{1,2,3}); // 컴파일 에러 (std::cout << vector 불가)

            print_integer(10);       // ✅ OK: int는 정수형
            // print_integer(3.14);  // ❌ 컴파일 에러: double은 정수형이 아님

            // 맴버함수에 requires 사용
            Printer<int>{}.print(10);      // 정수형
            Printer<double>{}.print(3.14); // 실수형

            // requires 절을 별도 표현식으로 사용
            display(10);       // 4바이트 이하 (int)
            display(3.141592); // 4바이트 초과 (double)
        }

    }


    void Test()
    {
        Concepts();
    }
}//Concepts