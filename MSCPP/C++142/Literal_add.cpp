#include "stdafx.h"


#include <format>        // C++20: std::format
#include <string_view>
#include <cstdint>       // For char8_t


namespace Literal_AddFeatures
{
	void StringLiterals_AddFeatures()
	{
		/*
			📚 String Literals 주요 추가 기능 (C++17)

			  - 문자열 리터럴 그 자체보다는 문자열 처리의 효율성과 표현력을 확장하는 기능이 강화

			  🔹 주요 기능 요약:

			    ✅ 1. `std::string_view` 타입 도입
				  - 문자열을 복사하지 않고 읽기 전용으로 참조할 수 있는 경량 문자열 뷰
				  - 포인터 + 길이로 구성된 구조체
				  - 매우 빠른 성능과 낮은 오버헤드 제공

				✅ 2. string_view 리터럴 접미사 `"..."sv` 도입
				  - `"abc"sv`        → `std::string_view`
				  - `u"abc"sv`       → `std::u16string_view`
				  - `U"abc"sv`       → `std::u32string_view`
				  - `L"abc"sv`       → `std::wstring_view`
				  ※ `using namespace std::string_view_literals;` 또는 `std::literals` 필요

			  🔹 활용 예:
			    using namespace std::literals;
				std::string_view sv = "hello"sv;

			  🔹 장점:
				- 복사 비용 없이 문자열을 참조
				- 함수 인자로 넘길 때 매우 효율적
				- 리터럴 → 객체 생성 과정 없이 즉시 뷰로 사용 가능

			  🔹 주의사항:
				- string_view는 null-terminated가 아님 (`'\0'` 보장 X)
				- 수명 관리 주의 필요 (임시 문자열 참조하면 댕글링 포인터 발생 가능)
		*/
		{
			using namespace std;
			using namespace std::string_literals;

			// 1. std::string_view 리터럴
			std::string_view sv1 = "C++17 string view"sv;
			std::u16string_view sv2 = u"유니코드"sv;
			std::u32string_view sv3 = U"문자열뷰"sv;
			std::wstring_view wsv = L"와이드뷰"sv;

			// 2. string_view는 부분 문자열 슬라이싱 가능
			std::string_view sub = sv1.substr(5, 6);  // "7 stri"

			// 출력
			std::cout << "[string_view]        : " << sv1 << "\n";
			std::cout << "[substring (5~10)]   : " << sub << "\n";
			std::wcout << L"[wstring_view]       : " << wsv << L"\n";
			std::cout << "[u16string_view size]: " << sv2.size() << "\n";
			std::cout << "[u32string_view size]: " << sv3.size() << "\n";

			/*
			출력:
				[string_view]        : C++17 string view
				[substring (5~10)]   : 7 stri
				[wstring_view]       : 와이드뷰
				[u16string_view size]: 4
				[u32string_view size]: 5
			*/
		}

		system("pause");
	}

	void Test()
	{
		StringLiterals_AddFeatures();
	}

}//Literal_AddFeatures