// C++141.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Function.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Logic::Test();

	//=============================================================================================

	CompilerAddFeatures::Test();
	Constexpr_AddFeature::Test();
	NSDMI_AddFeature::Test();

	SFINAE_AddFeature::Test();
	STL_Improvements::Test();
	VCPP_Improvements::Test();

	VCPP_REST_SDK::Test();
	VCPP_RunTime::Test();
	VCPP_Package::Test();

    return 0;
}

