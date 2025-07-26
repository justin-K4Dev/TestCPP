#include "stdafx.h"


#include <iostream>
#include <variant>
#include <string>


namespace Variant
{
	void variant_use()
	{
		/*
			📚 std::variant<Ts...>

			  - C++17에서 도입된 "여러 타입 중 한 가지만 가질 수 있는 타입 안전 유니언"
			  - 값이 항상 반드시 존재하며, 등록된 타입 중 하나로 저장
			  - 타입에 따라 동적으로 저장하고, 안전하게 꺼낼 수 있음
			  - std::visit로 타입별 분기 처리, std::get, std::get_if로 안전한 접근
			  - switch-like 패턴 매칭, 타입 안전성, 런타임 타입 체크가 강점
		*/

		std::variant<int, std::string, double> v; // int, string, double 중 하나
		v = 10;
		std::cout << "v holds int: " << std::get<int>(v) << std::endl; // 출력: v holds int: 10

		v = std::string("abc");
		std::cout << "v holds string: " << std::get<std::string>(v) << std::endl; // 출력: v holds string: abc

		v = 3.14;
		// 타입 안전 분기
		std::visit([](auto&& arg) {
			std::cout << "visit: value = " << arg << std::endl;
		}, v); // 출력: visit: value = 3.14

		// 안전한 타입 확인
		if (std::holds_alternative<double>(v))
			std::cout << "v is double!" << std::endl; // 출력: v is double!
	}

	void Test()
	{
		variant_use();
	}
}//end of namespace