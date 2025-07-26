// TestC++0x.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Function.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Logic::Test();

	//=============================================================================================

	Alias::Test();

	Constructor::Test();

	Inheritance_AddFeatures::Test();

	Lambda_AddFeature::Test();

	NSDMI::Test();

	RValueReferenceWithVariadicTemplate::Test();

	SFINAE_AddFeatures::Test();

	String_AddFeatures::Test();

	Thread_AddFutures::Test();

	TypeConversion::Test();

	VariadicTemplate::Test();
	
	return 0;
}

