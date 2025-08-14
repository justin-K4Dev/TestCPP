#include "stdafx.h"



namespace Literal_AddFeatures
{
	// 사용자 정의 리터럴(User-defined Literal, UDL) : 문자열 길이 반환
	constexpr int operator"" _toLength(const char* str, size_t len) {
		return static_cast<int>(len);
	}

	void StringLiterals_AddFeatures()
	{
		/*
			📚 String Literal 추가 기능 (C++11)

			  - 문자열 처리와 관련하여 다음과 같은 기능이 추가

			  🔹 1. Unicode 문자열 리터럴 (접두사)
				- `u"..."` → UTF-16 문자 배열 (`const char16_t*`)
				- `U"..."` → UTF-32 문자 배열 (`const char32_t*`)
				- `L"..."` → 기존 wide 문자 (`const wchar_t*`)

			  🔹 2. Raw String Literal (원시 문자열 리터럴)
				- 구문: `R"(내용)"`
				- 이스케이프 문자를 해석하지 않고 문자열을 그대로 저장
				- 예: `R"(C:\Users\Name\nFolder)"` → 그대로 출력

			  🔹 3. 사용자 정의 리터럴(User-defined Literal, UDL)
			    - 리터럴 뒤에 `_suffix` 형태로 사용자 정의 함수를 연결 가능
				- `constexpr`과 함께 사용 시 컴파일 타임 연산 가능 (C++14부터 강화됨)
				- 함수 형식 예:
					constexpr int operator"" _toLength(const char* s, size_t len);
		*/
		{
			// 1. UTF-16/32 유니코드 문자열 리터럴
			const char16_t* u16 = u"안녕하세요";  // UTF-16 문자열
			const char32_t* u32 = U"세계";       // UTF-32 문자열
			const wchar_t* wide = L"와이드";

			// 2. Raw String Literal
			const char* raw = R"(파일 경로: C:\User\Desktop\test.txt\n줄바꿈 없음)";

			// 3. 사용자 정의 리터럴
			constexpr int len = "StringLength"_toLength;

			// 출력
			std::wcout << L"[wchar_t*]         : " << wide << std::endl;
			std::cout << "[Raw String]        : " << raw << std::endl;
			std::cout << "[User-defined _len] : " << len << " characters\n";
			std::cout << "[UTF-16 size]       : " << sizeof(*u16) << " bytes\n";
			std::cout << "[UTF-32 size]       : " << sizeof(*u32) << " bytes\n";
			/*
			출력:
				[wchar_t*]          : 와이드
				[Raw String]        : 파일 경로: C:\User\Desktop\test.txt\n줄바꿈 없음
				[User-defined _len] : 13 characters
				[UTF-16 size]       : 2 bytes
				[UTF-32 size]       : 4 bytes	
			*/

			system("pause");
		}
	}

	void Test()
	{
		StringLiterals_AddFeatures();
	}

}//Literal_AddFeatures