#include "stdafx.h"



namespace FoldExpression
{
	template<typename... Args>
	auto RightFold(Args... args)
	{
		return (args - ...);
	}

	template<typename... Args>
	auto LeftFold(Args... args)
	{
		return (... - args);
	}

	template<typename... Args>
	auto RightFoldWithInit(Args... args)
	{
		return (args - ... - 1);
	}

	template<typename... Args>
	auto LeftFoldWithInit(Args... args)
	{
		return (1 - ... - args);
	}

	void fold_expression()
	{
		/*
			📚 Fold Expression (C++17)

			  - Fold Expression은 C++17에서 추가된 문법으로,
			    가변 인자 템플릿(Variadic Templates) 함수에서
			    연산자를 반복 적용(누적)하는 코드를 매우 간결하게 작성할 수 있게 해줍니다.

			  - 크게 4가지 형태가 있음:
			    [1] Unary Right Fold:   (args op ...)
			    [2] Unary Left Fold:    (... op args)
			    [3] Binary Right Fold:  (args op ... op init)
			    [4] Binary Left Fold:   (init op ... op args)

			  * 여기서 op는 +, -, *, &&, || 등 이항 연산자
			  * init은 초기값(Identity element), 예: 0, 1, true, false 등

			  - 실제 효과:
			    - (args - ...):        (a1 - (a2 - (a3 - a4)))
			    - (... - args):        (((a1 - a2) - a3) - a4)
			    - (args - ... - init): (a1 - (a2 - (a3 - (a4 - init))))
			    - (init - ... - args): (((init - a1) - a2) - a3) - a4

			  - Fold Expression은 재귀 없이 가변 인자 연산을 쉽고, 안전하고, 직관적으로 작성할 수 있게 해줍니다.
		*/

		std::cout << RightFold(1, 2, 3, 4) << std::endl;            // (1-(2-(3-4)))    // Unary Right simple examples
		std::cout << LeftFold(1, 2, 3, 4) << std::endl;             // (((1-2)-3)-4)    // Unary Left simple examples
		std::cout << RightFoldWithInit(2, 3, 4) << std::endl;       // (2-(3-(4-"1")))  // Binary Right simple examples
		std::cout << LeftFoldWithInit(2, 3, 4) << std::endl;        // ((("1"-2)-3)-4)  // Binary Left simple examples

		std::cout << std::endl;
	}

	void Test()
	{
		fold_expression();
	}

}//end of namespace