#include "stdafx.h"

#include "CRT_MemoryCheck.h"

namespace CRTMemoryCheck
{
	void check_memory_leak()
	{
		SET_CRT_MEMORY_CHECK(CRT_MEMORY_LEAK_ALWAYS_CHECK_FLAGS);

		{
			int *pIntByNew = new int;

			int *pIntByMalloc = (int*)malloc(sizeof(int));
		}

		_CrtDumpMemoryLeaks();

		system("pause");
	}


	void Test()
	{
		//check_memory_leak();
	}

}// end of CRTMemoryCheck