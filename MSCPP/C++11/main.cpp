// TestC++0x.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Function.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Logic::Test();

	//=============================================================================================

	ABAProblem::Test();

	AsyncAndFuture::Test();

	Auto::Test();

	CAS::Test();

	Chrono::Test();

	Constexpr::Test();

	DCAS::Test();

	DeclType::Test();

	Emplacement::Test();

	Enum::Test();

	Exception_AddFeatures::Test();

	Lambda::Test();

	Literal_AddFeatures::Test();

	Loop::Test();

	Memory_AddFeatures::Test();

	MemoryBarrier::Test();

	Mutex::Test();

	NullPtr::Test();

	Random_AddFeatures::Test();

	RegularExpression::Test();

	RValueReference::Test();

	StaticAssert::Test();

	TrailingReturnType::Test();

	Thread::Test();

	Tuple::Test();

	TypeInfo_AddFeatures::Test();
	
	return 0;
}

