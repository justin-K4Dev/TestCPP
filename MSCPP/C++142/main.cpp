// C++142.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "stdafx.h"
#include "Function.h"



int _tmain(int argc, _TCHAR* argv[])
{
	Logic::Test();

	//=============================================================================================

	Attribute::Test();

	Compiler_AddFeatures::Test();

	DeductionGuide::Test();

	FoldExpression::Test();

	GeneralizedRangeBasedForLoops::Test();

	Lambda_AddFeatures::Test();

	Literal_AddFeatures::Test();

	Memory_AddFeature::Test();

	Mutex_AddFeature::Test();

	IfSwitch_AddFeatures::Test();

	Invoke_AddFeatures::Test();
	
	Optional::Test();

	SFINIAE_AddFeatures::Test();

	STL_Improvements::Test();

	StructuredBindings::Test();

	TerseStaticAssert::Test();

	Tuple_AddFeatures::Test();

	TypeTraits_AddFeature::Test();

	Variant::Test();

	return 0;
}

