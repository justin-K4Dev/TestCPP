#include "stdafx.h"



namespace Lambda_AddFeatures
{
	void Lambda_AddFeatures()
	{
		/*
			📚 Lambda 관련 추가 기능 (C++17)

			  [3] constexpr 람다
			    - 람다 자체가 constexpr, noexcept 로 더 자유롭게 사용 가능.
		*/
		{
			// [1] 람다 내에서 constexpr 등 다양한 조합 가능
			constexpr auto square = [](auto v) constexpr { return v * v; };
			std::cout << square(5) << std::endl; // 25

			system("pause");
		}
	}

	void Test()
	{
		Lambda_AddFeatures();
	}

}//Lambda_AddFeatures