#include "stdafx.h"

#include <iostream>


namespace StaticAssert
{
	//use static_assert example
	const int MAX_LEVEL = 100;

	template< typename T1, int StackSize >
	class CMyStack
	{
		static_assert(StackSize >= 10, "Stack Size Error");

	public:
		T1* data;
		int nSize;

	public:
		CMyStack() : data(new T1[StackSize]), nSize(StackSize) { }
	};

	void static_assert_what()
	{
		/*
			📚 static_assert

			  - static_assert는 C++11에서 도입된 컴파일 타임 상수 조건 검사 구문
			  - 조건이 거짓이면 컴파일 에러와 함께 메시지를 출력 (런타임이 아닌 컴파일 타임에 에러 발생)
			  - 주로:
			    + 상수값 범위 체크
			    + 타입/크기 체크
			    + 템플릿 파라미터 제약
			    + 컴파일 환경/플랫폼 종속성 체크 등에 사용

			  - 실수로 잘못된 상수/파라미터/구조체 크기 등으로 인한 런타임 오류를 컴파일 타임에 사전 방지
		*/
		{
			//const value check 
			//static_assert(MAX_LEVEL <= 50, "Warning - MAX_LEVEL"); // compile error 발생

			//template const value check
			//CMyStack< int, 5 > myStack; // compile error 발생
			//std::cout << "StackSize: " << *myStack.nSize << std::endl;

			//value size check
			static_assert(sizeof(int) == 4, "not int size 4");

			system("pause");
		}
	}


	void Test()
	{
		static_assert_what();
	}

}//StaticAssert