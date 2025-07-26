// C++142.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "stdafx.h"
#include "Function.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Logic::Test();

	//=============================================================================================

	Compiler_AddFeatures::Test();

	DeductionGuide::Test();

	FallThrough::Test();

	FoldExpression::Test();

	GeneralizedRangeBasedForLoops::Test();

	Lambda_AddFeatures::Test();

	Literal_AddFeatures::Test();

	IfSwitch_AddFeatures::Test();
	
	Optional::Test();

	SFINIAE_AddFeatures::Test();

	STL_Improvements::Test();

	StructuredBindings::Test();

	TerseStaticAssert::Test();

	Tuple_AddFeatures::Test();

	Variant::Test();

	return 0;
}

