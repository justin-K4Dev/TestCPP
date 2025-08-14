#include "stdafx.h"


#include <iostream>
#include <string>
#include <chrono>
#include <thread>


namespace Literal_AddFeature
{
	void StringLiterals_AddFeature()
	{
		/*
			📚 String Literals 주요 추가 기능 (C++14)

			  - 문자열 리터럴 자체보다는 리터럴의 활용성과 확장성을 위한 기능이 크게 향상

			  🔹 주요 기능:

				✅ 1. `std::string` 리터럴 (`"..."s`)
				  - 접미사(suffix) `"s"`를 붙이면 `std::string` 객체로 바로 변환 가능
				  - `"hello"s` → `std::string`
				  - `u"..."s`, `U"..."s`, `L"..."s` 도 각각 `u16string`, `u32string`, `wstring` 생성
				  - 네임스페이스: `std::string_literals` 또는 `std::literals`

				✅ 2. `std::chrono` 시간 리터럴
				  - `10s`, `500ms`, `3h` 등으로 시간 표현 가능
				  - 타입: `std::chrono::seconds`, `milliseconds`, `hours` 등
				  - 네임스페이스: `std::chrono_literals`

				✅ 3. 사용자 정의 리터럴의 `constexpr` 지원 강화
				  - `operator"" _suffix()`를 `constexpr`로 선언 가능
				  - 리터럴 연산을 컴파일 타임에 수행할 수 있음

				🔹 예시:

				  using namespace std::string_literals;
				  std::string name = "C++"s + "14"s;

				  using namespace std::chrono_literals;
				  auto timeout = 500ms;

				🔹 주의 사항:
				  - `"..."s`, `"..."ms` 등의 표준 리터럴을 사용하려면 반드시 해당 네임스페이스를 포함해야 함
				  - 사용자 정의 리터럴은 `_suffix` 형태의 함수를 직접 정의해야 함
		*/
		{
			using namespace std;
			using namespace std::string_literals;
			using namespace std::chrono_literals;

			// 📌 1. std::string 리터럴
			std::string s = "C++"s + "14"s;
			std::wstring ws = L"넓은 문자열"s;
			std::u16string u16s = u"UTF16"s;
			std::u32string u32s = U"UTF32"s;

			// 📌 2. std::chrono 시간 리터럴
			auto waitTime = 1000ms;  // 1초

			// 📌 출력
			std::cout << "[std::string]       : " << s << "\n";
			std::cout << "[chrono wait]       : waiting 1 second...\n";
			std::this_thread::sleep_for(waitTime);
			std::cout << "[wait complete]\n";
			std::cout << "[u16 size]          : " << u16s.size() << "\n";
			std::cout << "[u32 size]          : " << u32s.size() << "\n";

			/*
			출력:
				[std::string]       : C++14
				[chrono wait]       : waiting 1 second...
				[wait complete]
				[user-defined _len] : 11 characters
				[u16 size]          : 5
				[u32 size]          : 5
			*/

			system("pause");
		}

	}

	void StringLiterals_suffix()
	{
		/*
			📚 String Literals Suffix (C++20)

			  - 문자열 리터럴 뒤에 붙는 접미사(suffix)를 통해 문자열의 타입을 명확히 지정하거나
				사용자 정의 동작을 부여할 수 있습니다.
			  - C++20에서는 `char8_t`, `std::u8string`, `std::format`, `string_view` 등과의
				조합을 통해 더 풍부한 문자열 처리 방식이 제공됩니다.

			  🔹 표준 접미사 종류:

				✅ 1. 기본 리터럴 접두사 (타입 지정용)
				   - `"abc"`         → `const char*`
				   - `L"abc"`        → `const wchar_t*`
				   - `u8"abc"`       → `const char8_t*`   (C++20부터 분리된 타입)
				   - `u"abc"`        → `const char16_t*`
				   - `U"abc"`        → `const char32_t*`
				   - `R"(raw\nstr)"` → Raw 문자열 (이스케이프 무시)

				✅ 2. `"abc"s`  (std::string 계열 리터럴)
				   - `"abc"s`       → `std::string`
				   - `L"abc"s`      → `std::wstring`
				   - `u8"abc"s`     → `std::u8string`    (C++20)
				   - `u"abc"s`      → `std::u16string`
				   - `U"abc"s`      → `std::u32string`
				   ※ 사용을 위해 `using namespace std::literals;` 필요

				✅ 3. `"abc"sv` (string_view 리터럴)
				   - `"abc"sv`      → `std::string_view`
				   - `u"abc"sv`     → `std::u16string_view`
				   - `U"abc"sv`     → `std::u32string_view`
				   ※ 복사 없이 문자열 참조할 때 유용
				   ※ `using namespace std::literals;` 필요

				✅ 4. 사용자 정의 리터럴
				   - `"hello"_len` → 사용자 정의 함수에서 길이 계산 등 구현 가능
				   - C++11 이상에서 사용 가능

			  🔹 예시:
				 using namespace std::literals;

				 std::string			s  = "abc"s;
				 std::u8string		u8 = u8"abc"s;
				 std::string_view	sv = "abc"sv;

			  🔹 주의사항:
				 - `u8"..."` 리터럴은 C++20부터 `char8_t` 타입이 되며, 기존 `const char*`와 호환되지 않음
				 - `"abc"s`, `"abc"sv` 등의 리터럴은 반드시 네임스페이스 `std::literals` 포함 필요
				 - 컴파일러는 `-std=c++20` 플래그 필요 (GCC 13+, Clang 16+, MSVC 2019 16.10+)

		*/
	}


	void Test()
	{
		StringLiterals_suffix();

		StringLiterals_AddFeature();
	}

}//Literal_AddFeature