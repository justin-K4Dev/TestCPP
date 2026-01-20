#include "stdafx.h"


#include <iostream>
#include <string>
#include <format>        // C++20: std::format
#include <string_view>
#include <type_traits>   // std::is_same_v
#include <cstdint>       // For char8_t


namespace Literal_AddFeatures
{
    // ✅ 4. 컴파일 타임 문자열 길이 측정용 consteval 함수
    consteval int compileTimeStrLen(const char* str)
    {
        int len = 0;
        while (str[len] != '\0') ++len;
        return len;
    }

    void StringLiterals_AddFeature()
    {
        /*
            📚 String Literal 주요 추가 기능 (C++20)

              - 소스 코드 안에 직접 작성된 고정된 문자열 값입니다. 말 그대로 "문자열 그 자체를 리터럴로 표현한 것"
              - 컴파일 타임에 메모리에 저장되며, 주로 `const char*` 또는 다양한 인코딩 타입으로 사용됨.

              🔹 주요 기능:

                ✅ 1. char8_t 타입 정식 도입 (UTF-8 문자열 전용)
                    - 기존 u8"" 리터럴의 타입이 `const char*` → `const char8_t*`로 변경됨
                    - 명확한 UTF-8 문자열 타입 지원
                    - 다른 문자열 타입과는 명시적 변환 필요

                ✅ 2. std::format 도입 (Python 스타일 문자열 포맷)
                    - <format> 헤더 추가
                    - 타입 안전한 문자열 포맷팅 지원: std::format("Name: {}, Score: {}", name, score)

                ✅ 3. Raw Literal과의 접두사 병행 가능 (e.g., u8R"()" 형태)
                    - 복잡한 문자열에서 escape 없이 입력 가능 + 인코딩 지정

                ✅ 4. consteval / constinit과 결합한 컴파일 타임 문자열 처리
                    - 컴파일 타임에 문자열 길이 등의 분석 가능
                    - `static_assert` 등과 결합 시 정적 검증 가능

                ✅ 5. string_view + ranges 연계 (뷰 기반 문자열 조작)
                    - string_view와 std::views::transform 등 연계로 성능 개선

                ✅ 6. string_view 리터럴 접미사(suffix) ("..."sv)
                    - 문자열을 복사하지 않고 읽기 전용 참조 형태로 다룰 수 있음
                    - `"abc"sv`     → `std::string_view`
                    - `u"abc"sv`    → `std::u16string_view`
                    - `U"abc"sv`    → `std::u32string_view`
                    - `L"abc"sv`    → `std::wstring_view`
                    - 매우 긴 문자열을 복사 없이 전달하거나, 부분 슬라이싱할 때 유리

                ⚠️ 주의사항:
                    - std::format은 GCC 13+, Clang 16+, MSVC 2019 16.10+ 이상 필요
                    - char8_t은 기존 const char* 함수들과 호환되지 않음
        */
        {
            using namespace std;
            using namespace std::string_literals;
            using namespace std::literals;

            // ✅ 1. char8_t 타입 리터럴
            const char8_t* utf8 = u8"UTF-8 문자열";  // C++20에서는 char8_t*
            cout << "[1. char8_t literal]     : sizeof(*utf8) = " << sizeof(*utf8) << " byte\n";

            // ✅ 2. std::format
            string name = "ChatGPT";
            int version = 4;
            string formatted = format("Hello, {} v{}!", name, version);
            cout << "[2. std::format]         : " << formatted << "\n";

            // ✅ 3. Raw UTF-16 Literal
            const char16_t* raw_utf16 = uR"(Line1\nLine2)";
            cout << "[3. raw UTF-16 literal]  : ";
            for (char16_t ch : u16string{ raw_utf16 }) cout << static_cast<char>(ch);
            cout << "\n";

            // ✅ 4. consteval 기반 문자열 길이 검증
            constexpr int len = compileTimeStrLen("CompileTime!");
            static_assert(len == 12, "문자열 길이 오류!");

            cout << "[4. consteval length]    : " << len << "\n";

            // ✅ 5. string literal with "s" (std::string)
            string s = "C++20"s;
            cout << "[5. std::string literal] : " << s << "\n";

            // ✅ 6. string_view with "sv" (no copy)
            string_view sv = "no copy string_view"sv;
            string_view sliced = sv.substr(3, 4);  // "copy"
            cout << "[6. string_view literal] : " << sv << "\n";
            cout << "[6. sliced (3~6)]        : " << sliced << "\n";

            /*
            출력:
                [1. char8_t literal]     : sizeof(*utf8) = 1 byte
                [2. std::format]         : Hello, ChatGPT v4!
                [3. raw UTF-16 literal]  : Line1\nLine2
                [4. consteval length]    : 12
                [5. std::string literal] : C++20
                [6. string_view literal] : no copy string_view
                [6. sliced (3~6)]        : copy
            */
        }

        system("pause");
    }


	void Test()
	{
		//StringLiterals_AddFeature();
	}

}//Literal_AddFeatures