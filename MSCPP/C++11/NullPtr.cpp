#include "stdafx.h"


namespace NullPtr
{
	int func(double *p) { return 0; }

	void nullptr_const()
	{
		/*
			📚 nullptr (C++11)

			  - nullptr은 C++11에서 도입된 '타입이 있는 진짜 포인터 null 상수'
			  - 기존의 NULL(대개 0 또는 ((void*)0)으로 정의)은 int/long 등 정수 타입과 혼동 위험이 크고,
				오버로드 함수 호출, 템플릿 등에서 모호성/버그 유발이 잦았음
			  - nullptr은 "std::nullptr_t" 타입이며, 오직 포인터 타입과만 비교/대입 가능(정수/불리언 혼동 방지)
			  - 함수 오버로드, 포인터 인수 구분, 타입 안정성 향상에 매우 중요

			  ※ 현대 C++에서는 반드시 NULL 대신 nullptr 사용 권장!
		*/
		{
			// [1] 포인터 변수에 nullptr 초기화
			char *p = nullptr;

			// [2] 함수에 nullptr 전달(정수와 구분되는 타입 안전성)
			func(nullptr);

			// [3] nullptr의 크기(표준에선 구현마다 다름, 보통 void* 크기)
			std::cout << "sizeof(nullptr): " << sizeof(nullptr) << std::endl;

			// [4] nullptr의 타입 정보
			std::cout << "typeid(nullptr).name(): " << typeid(nullptr).name() << std::endl;

			// [5] NULL과 nullptr은 비교시 같음(값은 같지만, 타입은 다름)
			if (NULL == nullptr) {
				std::cout << "equal NULL == nullptr" << std::endl;
			}

			// [6] nullptr은 정수(0)와 비교 불가(컴파일 오류)
			// if (nullptr == 0) ... // X

			// [7] nullptr은 true/false와 비교 불가(컴파일 오류)
			// if (nullptr) ... // X

			// [8] 예외 throw 시에도 타입 명확히 전달
			// throw nullptr; // 포인터 예외

			system("pause");
		}
	}

	void Test()
	{
		nullptr_const();
	}

}//NullPtr