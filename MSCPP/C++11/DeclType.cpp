#include "stdafx.h"


namespace DeclType
{
	// decltype keyword example
	int foo5() { return 0; }

	void decltype_what()
	{
		/*
			📚 decltype 키워드 개요 (C++11~)

			  - decltype은 표현식(변수, 연산, 함수 호출 등)의 실제 타입을 컴파일 타임에 추론해서
				그 타입을 선언에 그대로 쓸 수 있게 해주는 키워드
			  - 주로 변수 선언, 함수 반환 타입, 복잡한 타입 추론, 템플릿에서 매우 유용

			  - 활용 예시:
				• decltype(x) a;             // x와 동일한 타입
				• decltype(x + y) sum;       // x+y의 타입
				• decltype(foo()) val;       // foo()의 반환 타입

			  - 장점:
				• 타입 자동 추론, 템플릿/복잡한 연산 결과의 타입을 안전하게 선언 가능
				• 타입 명시 중복 제거, 가독성·유지보수성 향상

			  - 제한/주의:
				• "표현식의 타입"을 추론하므로, 레퍼런스/const 등 복잡한 타입도 그대로 따름
				• 함수 오버로드/표현식의 값종류(L/R value) 등도 반영됨

			  ※ C++11~ 표준부터 지원, auto와 함께 사용 시 강력함
		*/
		{
			// 변수의 타입으로 사용
			int nHP = 0;
			decltype (nHP) nNPCHP = 5; // int nNPCHP = 5 과 동일
			decltype (nHP + nNPCHP) nTotalHP = 10; // int nTotalHP 와 동일
			decltype (nHP) *pnHP = &nHP; // int *pnHP 와 동일

			// 함수의 반환 타입으로 사용
			decltype (foo5()) value = 100;
		}
	}

	
	void Test()
	{
		decltype_what();
	}

}//DeclType