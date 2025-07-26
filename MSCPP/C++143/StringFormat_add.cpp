#include "stdafx.h"

#include <format>
#include <vector>


namespace StringFormat_AddFeatures
{
	void StringFormat_AddFeatures()
	{
		/*
			📚 String Format 주요 추가 기능
		
		      - C++20에서 새롭게 도입된 **타입-세이프(type-safe)** 문자열 포맷팅 함수
			  - 기존 `printf`, `std::stringstream`의 한계를 보완한 현대적 포맷 기능 제공
			  - Python의 `str.format()`, C#의 `string.Format()`과 유사한 스타일
			  - 컴파일 타임 타입 체크 → 안정적이고 가독성이 뛰어난 코드 작성 가능

			  🔹 주요 기능:
			    - `{}` 위치 지정자 기반 포매팅
				- 정수, 실수, 문자열, 정렬, 채움 문자, 숫자 진법 등 다양한 옵션 지원
				- `std::string`, `std::wstring` 모두 지원 (UTF-8/유니코드 호환)
				- 사용자 정의 타입도 `std::formatter<T>` 특수화를 통해 포맷 확장 가능

			  🔹 사용 예:
				- std::format("num: {}, pi: {:.2f}", 42, 3.14159);         // num: 42, pi: 3.14
				- std::format("{:>10}", "hello");                          // 오른쪽 정렬
				- std::format("{:08X}", 255);                              // 0으로 채운 16진수

			  🔹 관련 함수들:
				- std::format(...)           → 문자열 반환
				- std::format_to(...)        → 반복자에 직접 출력
				- std::format_to_n(...)      → 최대 N글자까지 출력
				- std::vformat(...)          → 가변 인자 리스트 포맷 (런타임 포맷)

			  🔹 주의 사항:
				- `<format>` 헤더 필요
				- Visual Studio 2022, GCC 13+, Clang 16+ 이상에서 사용 가능
				- 구버전 GCC/Clang에서는 `fmtlib`(외부 라이브러리) 사용 필요
		*/
	}

	void format_use()
	{
		/*
			📚 std::format()
		*/
		{
			int num = 42;
			double pi = 3.14159;
			std::string msg = std::format("num: {}, pi: {:.2f}", num, pi);
			std::cout << msg << std::endl;
			// 출력: num: 42, pi: 3.14
		}
		{
			std::cout << std::format("{} + {} = {}", 1, 2, 1 + 2) << "\n";
			// 출력: 1 + 2 = 3
		}

		system("pause");
	}

	void format_to_use() 
	{
		/*
			📚 std::format_to()
		
			  - 지정한 출력 반복자(Output Iterator)에 포맷 결과를 직접 출력
			  - std::string, std::vector<char> 등 기존 버퍼에 이어쓰기 가능
			  - 반환값: 마지막에 쓴 위치의 반복자
		*/
		{
			std::string buf;
			std::format_to(std::back_inserter(buf), "ID: {}, Name: {}", 7, "Justin");
			std::cout << buf << std::endl;
			// 출력: ID: 7, Name: Justin
		}

		system("pause");
	}

	void format_to_n_use() 
	{
		/*
			📚 std::format_to_n()

			  - 최대 n개의 문자까지만 포맷 결과를 Output Iterator에 출력
			  - 반환값: {마지막 반복자, 실제 쓴 문자 수}
		*/
		{
			std::vector<char> buf(15); // 15글자까지 출력할 수 있도록 버퍼 준비
			auto result = std::format_to_n(buf.begin(), buf.size(), "abcdefghijklmno{}{}", 1, 2);
			// 실제로 버퍼에 들어간 문자열은 널 문자가 없으므로 std::string으로 변환시킴
			std::string out(buf.data(), result.out - buf.begin());
			std::cout << out << std::endl;
			// 출력: abcdefghijklmno
			// (포맷 결과가 잘려서 15글자까지만 출력됨)
			std::cout << "실제 출력된 글자 수: " << result.size << std::endl;
			// 출력: 실제 출력된 글자 수: 15
		}

		system("pause");
	}

	void vformat_use() 
	{
		/*
			std::vformat()
			
			- 인자 리스트(std::format_args)를 받아 동적으로 포맷 수행
			- 런타임에 인자 개수가 정해질 때 사용
			- 내부적으로 std::make_format_args를 활용
		*/
		{
			std::string name = "vformat";
			int n = 2025;
			// 가변 인자를 묶어서 전달
			auto msg = std::vformat("API: {}, Year: {}", std::make_format_args(name, n));
			std::cout << msg << std::endl;
			// 출력: API: vformat, Year: 2025
		}

		system("pause");
	}

	void format_options_use()
	{
		/*
			📚 std::format() 포맷팅 옵션

			  - 정렬: {:<}, {:>}, {:^} → 좌/우/중앙 정렬
			  - 폭 지정: {:10} → 최소 10칸
			  - 채움 문자: {:*<10} → '*' 문자로 채움
			  - 정수 진법 출력: {:b}, {:o}, {:x}, {:X} → 2/8/16진수
			  - 부동소수점 자리수: {:.2f} → 소수점 이하 2자리
			  - 부호 표시: {:+d} → 양수에도 + 표시
			  - 그룹 구분자: {:L} → 지역화된 숫자 표기 (ex. 1,000)

			  💡 복합 예시: {:*>10.2f} → 10자리, 소수점 2자리, 오른쪽 정렬, '*' 채움
		*/

		{
			std::cout << std::format("왼쪽 정렬:  [{:<10}]\n", "Left");
			std::cout << std::format("오른쪽 정렬: [{:>10}]\n", "Right");
			std::cout << std::format("중앙 정렬:  [{:^10}]\n", "Center");
			std::cout << std::format("채움 문자:  [{:*^10}]\n", "Pad");
		}
		{
			int n = 42;
			std::cout << std::format("이진수  : {:b}\n", n);
			std::cout << std::format("8진수   : {:o}\n", n);
			std::cout << std::format("16진수  : {:x}\n", n);
			std::cout << std::format("16진수(대): {:X}\n", n);
		}
		{
			double d = 1234.56789;
			std::cout << std::format("소수점 제한: {:.2f}\n", d);
			std::cout << std::format("폭 지정+채움: [{:*>12.2f}]\n", d);
		}
		{
			int pos = 77, neg = -77;
			std::cout << std::format("부호 출력: {:+d}, {:+d}\n", pos, neg);
		}

		system("pause");
	}

	void Test()
	{
		format_options_use();

		vformat_use();

		format_to_n_use();

		format_to_use();

		format_use();

		StringFormat_AddFeatures();
	}

}//StringFormat_AddFeatures