#include "stdafx.h"

#include <vector>
#include <atomic>
#include <ctime>
#include <time.h>
#include <chrono>
#include <Windows.h>

namespace TrailingReturnType
{
	template<typename T, typename U>
	auto add(T a, U b) -> decltype(a + b) {
		return a + b;
	}

	void TrailingReturnType_what()
	{
		/*
			📚 트레일링 리턴 타입(Trailing Return Type) 개요 (C++11~)

			  - 함수 선언에서 반환 타입을 괄호 뒤에 '-> 반환타입' 형식으로 명시하는 C++11 표준 문법
			  - 주로 템플릿 함수에서 파라미터에 의존하는 복잡한 타입(예: decltype(a + b))을 반환해야 할 때 사용

				예시:
					auto func(파라미터...) -> 반환타입 { ... }

			  - 장점:
				• 파라미터 선언 이후에 반환 타입을 지정 가능
				• decltype, 타입 연산, 템플릿 가변 인자 등에서 타입 추론의 자유로움/가독성 향상
				• 람다 표현식의 반환 타입 명시에도 동일하게 활용

			  - 적용 표준:
				• C++11~ (그 이전 버전에서는 지원하지 않음)

			  - 활용:
				• 템플릿/가변 인자 함수, 타입 연산/추론 함수, 람다 등 현대 C++ 고급 패턴에서 필수 도구
		*/
		{
			// int + int
			std::cout << add(2, 3) << std::endl;             // 5

			// int + double
			std::cout << add(3, 4.5) << std::endl;           // 7.5

			// double + double
			std::cout << add(2.5, 7.5) << std::endl;         // 10

			// std::string + const char*
			std::string s = "Hello, ";
			std::cout << add(s, "world!") << std::endl;      // Hello, world!

			// char + int
			std::cout << add('A', 2) << std::endl;           // 'A' == 65, so 67

			// float + int
			std::cout << add(1.5f, 2) << std::endl;          // 3.5

			/*
			출력:
				5
				7.5
				10
				Hello, world!
				67
				3.5
			*/

			system("pause");
		}
	}


	void Test()
	{
		TrailingReturnType_what();
	}
}//TrailingReturnType