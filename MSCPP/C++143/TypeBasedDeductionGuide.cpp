#include "stdafx.h"


#include <iostream>
#include <vector>
#include <type_traits>


namespace TypeBasedDeductionGuide
{
	template<typename T>
	struct Box {
		T value;
		Box(T v) : value(std::move(v)) {}
	};

	// std::string 전용 Box (일반화된 생성자)
	template<>
	struct Box<std::string> {
		std::string value;
		template <typename U>
		Box(U&& v) : value(std::forward<U>(v)) {}
	};

	Box(std::vector<int>)->Box<std::vector<int>>;
	Box(int)->Box<int>;
	Box(double)->Box<double>;

	// float 전용
	template<typename T, std::enable_if_t<std::is_same_v<T, float>, int> = 0>
	Box(T) -> Box<float>;

	// const char* 전용 (string으로 변환)
	template<typename T, std::enable_if_t<std::is_same_v<T, const char*>, int> = 0>
	Box(T) -> Box<std::string>;

	// 문자열 리터럴(const char[N]) 전용
	template<std::size_t N>
	Box(const char(&)[N]) -> Box<std::string>;



	void TypeBased_DeductionGuide()
	{
		/*
			📚 타입별 Deduction Guide (TypeBased Deduction Guide)

			  - C++20부터는 더 복잡한 타입이나 조건에 따라 여러 deduction guide를 명시적으로 둘 수 있습니다.
			  - SFINAE, requires, std::enable_if와 결합해 타입별로 별도의 가이드 지정 가능
			  - Deduction Guide란, 클래스 템플릿의 타입 파라미터를 생성자 인자만 보고 자동 유추하는 규칙을 의미
			  - 복잡한 타입 조건, 컨테이너/스칼라 구분 등 실무 활용 빈도가 높아짐 (특히 C++20 requires 활용)
		*/

		Box b1 = 123;
		Box b2 = 3.14;
		Box b3 = std::vector<int>{ 1,2,3 };
		Box b4 = "hello";  // 이제 OK! (const char[N] → Box<std::string>)
		Box b5 = 1.23f;
		Box b6 = std::string("world"); // std::string → Box<std::string>

		std::cout << "b1: " << b1.value << std::endl;              // 출력: b1: 123
		std::cout << "b2: " << b2.value << std::endl;              // 출력: b2: 3.14
		std::cout << "b3.size(): " << b3.value.size() << std::endl; // 출력: b3.size(): 3
		std::cout << "b4: " << b4.value << std::endl;              // 출력: b4: hello
		std::cout << "b5: " << b5.value << std::endl;              // 출력: b5: 1.23
		std::cout << "b6: " << b6.value << std::endl;              // 출력: b6: world
	}

	void Test()
	{
		//TypeBased_DeductionGuide();
	}
}