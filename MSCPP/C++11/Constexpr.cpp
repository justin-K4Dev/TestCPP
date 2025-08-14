#include "stdafx.h"

#include <vector>

namespace Constexpr
{
	// C++11 constexpr 함수 예시
	constexpr int add(int a, int b) {
		return a + b;   // 한 줄만 허용 (return문만 가능)
	}

	// 재귀도 가능(조건문은 삼항연산자로만!)
	constexpr int factorial(int n) {
		return n <= 1 ? 1 : n * factorial(n - 1);
	}

	void constexpr_what()
	{
		/*
			📚 constexpr

			  - constexpr은 컴파일 타임 상수(constant expression) 값을 지정하는 키워드입니다.
			  - constexpr 변수/함수는 반드시 컴파일 타임에 계산될 수 있어야 하며,
				복잡한 문장(반복문, 지역변수 등)은 사용할 수 없습니다.
			  - C++11에서는 constexpr 함수는 반드시 return 한 줄만 허용하며,
			    조건문, 반복문, 지역 변수 선언 등은 금지됩니다.
			  - 배열 크기, 상수 표현식, 템플릿 파라미터 등에서 자주 사용됩니다.
		*/
		{
			constexpr int v = add(10, 20);   // 컴파일 타임 계산
			std::cout << v << std::endl;     // 출력: 30

			constexpr int f = factorial(5);
			std::cout << f << std::endl;     // 출력: 120

			// 런타임 값은 사용할 수 없음 (상수로만 초기화 가능)
			// int x = 10;
			// constexpr int v2 = add(x, 20); // 오류: x는 상수 표현식이 아님

			system("pause");
		}
	}


	void Test()
	{
		constexpr_what();
	}

}//Constexpr