#include "stdafx.h"


#include <thread>


namespace TypeTraits_AddFeature
{
	//================================================================================================

	void is_void_v_what()
	{
		/*
			📚 std::is_void_v<T> (C++14)

			  - 타입 T가 void인지를 컴파일 타임에 판별하는 “타입 트레이트(type trait)”
			  - 결과 타입: bool (컴파일 타임 상수)
			  - true → T가 void (또는 const void, volatile void, const volatile void)
			  - false → 그 외 모든 타입
		*/
		{
			std::cout << std::is_void_v<void> << "\n";         // true
			std::cout << std::is_void_v<int> << "\n";          // false
			std::cout << std::is_void_v<const void> << "\n";   // true

			system("pause");
		}
	}

	//================================================================================================

	void Test()
	{
		is_void_v_what();
	}

}//TypeTraits_AddFeature