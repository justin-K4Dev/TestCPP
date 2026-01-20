#include "stdafx.h"



namespace DeductionGuide
{

	void Implicit_DeductionGuide()
	{
		/*
			📚 암시적 Deduction Guide (Implicit Deduction Guide)
			  - 컴파일러가 템플릿 생성자(constructor) 시그니처를 자동으로 이용해서
			    템플릿 인자를 추론하는 경우입니다.
			  - 사용자가 별도로 deduction guide를 명시하지 않아도 동작합니다.
		*/

		std::vector v{ 1,2,3 }; // std::vector<int>로 추론됨 (암시적 deduction guide)
		for (auto x : v) std::cout << x << " ";
		// 출력: 1 2 3

		system("pause");
	}

	//================================================================================================

	// CTAD 불가 상황 (명시적 deduction guide 필요)
	template<typename T>
	struct Wrapper {
		T value;
		Wrapper(T v) : value(v) {}
	};

	// deduction guide 명시적 정의
	template<typename T>
	Wrapper(T) -> Wrapper<T>;

	void Explicit_DeductionGuide()
	{
		/*
			📚 명시적 Deduction Guide (Explicit Deduction Guide)
              - 사용자가 직접 deduction guide를 정의하여
				템플릿 타입 인자가 어떻게 추론되어야 할지 지정합니다.
			  - 복잡한 템플릿 타입(특히 여러 인자 혹은 포인터 변환 등)에서 주로 사용합니다.
		*/

		Wrapper w1(42);					// Wrapper<int>
		Wrapper w2(std::string("hi"));	// Wrapper<std::string>
		std::cout << w1.value << " " << w2.value << "\n";
		// 출력: 42 hi

		system("pause");
	}

	//================================================================================================

	template<typename T>
	struct MyPair {
		T first, second;
		MyPair(T a, T b) : first(a), second(b) {}
	};

	// int 두 개는 int 타입, double 두 개는 double 타입, 아니면 std::string
	MyPair(int, int)->MyPair<int>;
	MyPair(double, double)->MyPair<double>;
	MyPair(const char*, const char*)->MyPair<std::string>;

	void TypeBased_DeductionGuide()
	{
		/*
			📚 타입별 Deduction Guide (TypeBased Deduction Guide) 
			  - C++20부터는 더 복잡한 타입이나 조건에 따라 여러 deduction guide를 명시적으로 둘 수 있습니다.
		*/

		MyPair p1(1, 2);					// MyPair<int>
		MyPair p2(1.5, 2.5);				// MyPair<double>
		MyPair p3("hi", "there");			// MyPair<std::string>
		std::cout << p1.first << "\n";      // 1
		std::cout << p2.first << "\n";      // 1.5
		std::cout << p3.first << "\n";      // hi
	}


	void Test()
	{
		TypeBased_DeductionGuide();

		Explicit_DeductionGuide();

		Implicit_DeductionGuide();
	}

}//DeductionGuide