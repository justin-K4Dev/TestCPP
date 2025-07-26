// C++143.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "stdafx.h"
#include "Function.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Logic::Test();

	//////////////////////////////////////////////////////////////////////////////

	Atomic_AddFeatures::Test();

	Bit::Test();

	Concepts::Test();

	Coroutine::Test();

	Explicit_AddFeatures::Test();

	Lambda_AddFeatures::Test();

	MemorySpan::Test();

	Modules::Test();

	NSDMI_AddFeatures::Test();
	
	StringFormat_AddFeatures::Test();

	Literal_AddFeatures::Test();

	ThreeWayCompare::Test();
	
	TypeBasedDeductionGuide::Test();

	VCPP_Improvements::Test();

	return 0;
}

