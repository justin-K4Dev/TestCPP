#include "stdafx.h"

#include <iostream>
#include <filesystem>
#include <fstream>



namespace Explicit_AddFeatures
{
    struct MyType 
    {
        int value;

        template <typename T>
        explicit(std::is_same_v<T, int>)  // int 타입일 경우에만 explicit
        MyType(T v) : value(static_cast<int>(v)) {}
    };

    void print(MyType t) 
    {
        std::cout << "MyType value: " << t.value << "\n";
    }

    void Explicit_AddFeatures()
    {
        /*
            📚 조건부 explicit 생성자 (C++20~)

              - C++20에서는 `explicit(true/false)` 형태로 생성자/변환자의 암시적 변환 허용 여부를 조건부로 제어 가능
              - 기존에는 `explicit` 키워드가 유무만 결정됐지만, 이제는 조건식으로 제어 가능

              🔸 주요 목적:
                - 템플릿 기반 생성자에서 타입 특성이나 정책에 따라 암시적 변환을 허용할지 말지 제어 가능
                - 타입 안전성과 편의성을 동시에 만족 가능

              🔸 문법:
                explicit(<bool constant expression>)
                조건식 (타입 체크 => true) → 암시적 변환 불가
                조건식 (타입 체크 => false) → 암시적 변환 허용

              🔸 예시 조건:
                - `std::is_convertible_v<From, To>`
                - 템플릿 파라미터 `EnableImplicit`

              🔸 적용 위치:
                - 생성자(Constructor)
                - 변환 연산자(operator Type())
        */
        {
            // ❌ 실패: T = int → explicit(true) → 암시적 변환 불가
            // MyType a = 10;

            // ✅ 명시적 호출만 허용
            MyType a(10);

            // ✅ T = double → explicit(false 아님) → 암시적 변환 허용
            MyType b = 3.14; // 3 으로 암시적 변환

            // ✅ 명시적 생성자 호출
            MyType c(3.14);

            /*
                * T = double인 경우 → explicit(false) → 암시적 변환 가능
                  ✅ MyType b = 3.14; 가능
                  ✅ MyType b(3.14); 가능

                * T = int인 경우 → explicit(true) → 암시적 변환 불가능
                  ❌ MyType a = 10; → 컴파일 오류
                  ✅ MyType a(10); → 명시적 호출은 허용            
            */

            print(a);
            print(b);

            /*
            출력:
                MyType value: 10
                MyType value: 3
            */
        }

        system("pause");
    }


    void Test()
    {
        //Explicit_AddFeatures();
    }
}//Explicit_AddFeatures