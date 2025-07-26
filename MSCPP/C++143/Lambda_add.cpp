#include "stdafx.h"



namespace Lambda_AddFeatures
{
	void TemplateParameters_in_Lambdas()
	{
		/*
			📚 Template Parameter List in Lambdas (C++20)

			  ✅ 개요:
				- C++20부터 람다에 명시적으로 템플릿 파라미터를 지정할 수 있음
				- 기존에는 auto를 사용한 generic 람다는 가능했지만, 명시적인 템플릿 타입은 지정 불가
				- 이제는 []<typename T>(T arg) { ... } 와 같은 문법 사용 가능
				- 함수 템플릿처럼 타입 제약, SFINAE, constexpr 조건 등을 직접 지정 가능
				- 복잡한 타입 추론이 필요한 경우 특히 유용

			  ✅ 문법:
				auto lambda = []<typename T>(T value) { ... };

			  ✅ 활용 예:
				- 다양한 타입에 대해 동작하는 람다를 명시적으로 정의
				- STL과의 조합 (e.g., std::ranges, std::visit)
				- 람다에 `requires` 또는 `concepts` 추가 가능 (C++20 concepts와 함께 사용)

			  ✅ 기존 generic lambda 와 차이점:
				auto l1 = [](auto x) { };     // 암묵적인 템플릿 파라미터
				auto l2 = []<typename T>(T x) { };  // 명시적인 템플릿 파라미터 (C++20)
		*/
		{
			// ✅ 명시적 템플릿 파라미터를 사용하는 람다 정의
			auto printTypeAndValue = []<typename T>(T value) {
				std::cout << "Type: " << typeid(T).name() << ", Value: " << value << "\n";
			};

			// 다양한 타입에 대해 호출 가능
			printTypeAndValue(42);          // int
			printTypeAndValue(3.14);        // double
			printTypeAndValue(std::string("Hello")); // std::string

			// ✅ 템플릿 매개변수 2개 사용 예시
			auto sum = []<typename T, typename U>(T a, U b) {
				return a + b;
			};

			std::cout << "sum(1, 2.5) = " << sum(1, 2.5) << "\n"; // double
			std::cout << "sum(std::string, const char*) = " << sum(std::string("A"), "B") << "\n";

			system("pause");
		}
	}


	void Test()
	{
		TemplateParameters_in_Lambdas();
	}
}//Lambda_AddFeatures