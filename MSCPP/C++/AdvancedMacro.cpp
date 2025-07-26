#include "stdafx.h"

#include "Colors.h"

namespace AdvancedMacro
{
	void enum_to_string()
	{
		{
			std::cout << ColorEnum_ToString(BLUE) << std::endl;
		}

		system("pause");

		/*
		output:
			BLUE
		*/
	}

	void Test()
	{
		//enum_to_string();
	}
}