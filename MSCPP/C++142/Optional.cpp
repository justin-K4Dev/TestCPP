#include "stdafx.h"


#include <iostream>
#include <optional>
#include <string>


namespace Optional
{

	void optional_use()
	{
		/*
			📚 std::optional<T>

			  - C++17에서 도입된 "값이 있을 수도, 없을 수도 있는" 컨테이너
			  - 함수의 반환값, 멤버 등에서 'null' 또는 '값 없음'을 명확하게 표현
			  - 값이 없을 때는 std::nullopt로 표현 (포인터 없이 안전하게 사용)
			  - 주요 함수: has_value(), value(), value_or(), emplace(), reset() 등
		*/

		std::optional<int> maybeInt;  // 값 없음(nullopt)
		if (!maybeInt.has_value())
			std::cout << "maybeInt is empty" << std::endl;   // 출력

		maybeInt = 42;   // 값 할당
		if (maybeInt)
			std::cout << "maybeInt = " << *maybeInt << std::endl; // 출력: maybeInt = 42

		std::optional<std::string> maybeStr = std::nullopt;
		maybeStr = "hello";
		std::cout << "maybeStr = " << maybeStr.value() << std::endl; // 출력: maybeStr = hello
	}

	void Test()
	{
		optional_use();
	}

}//Optional