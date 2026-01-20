#include "stdafx.h"



namespace TerseStaticAssert
{
	static_assert(sizeof(int) >= 4); // 메시지 없이 사용(C++17 이상)
	static_assert(std::is_integral_v<int>); // 메시지 생략 가능

	void terse_static_assert()
	{
		/*
			📚 Terse static_assert (C++17~)

			  - static_assert 문법의 두 번째 인수(에러 메시지)를 생략할 수 있게 된 C++17의 문법 개선
			  - 메시지를 쓰지 않아도 간단히 컴파일 타임 조건 검증 가능
			  - 사용 예:
				static_assert(cond);                  // C++17 이상에서 OK
				static_assert(cond, "error message"); // C++11~OK
			  - Visual Studio에서는 /std:c++17, /std:c++20, /std:c++latest 옵션에서 지원
		 	  - URL: http://en.cppreference.com/w/cpp/language/static_assert
		*/

		static_assert(sizeof(long) >= 4, "long too small"); // 메시지 포함도 여전히 가능
	}

	void Test()
	{
		terse_static_assert();
	}

}//TerseStaticAssert