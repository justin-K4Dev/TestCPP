#include "stdafx.h"



namespace SFINAE
{
	// ✅ 오버로드 대상 타입: int 타입만 허용
	template <typename T>
	void print_impl(T x, char (*)[sizeof((T*)0, 0)] = 0) {
		std::cout << "[generic] Value: " << x << "\n";
	}

	// ✅ 특수화: 포인터 타입만 허용
	template <typename T>
	void print_impl(T* x, int = 0) {
		std::cout << "[pointer] Addr: " << x << ", Value: " << *x << "\n";
	}

	void SFINAE_what()
	{
		/*
			📚 SFINAE (Substitution Failure Is Not An Error) (C++98~)

			  - SFINAE는 Substitution Failure Is Not An Error의 약자로, 
			    C++98 부터 언어 표준에 포함된 템플릿 메타프로그래밍에서 타입을 치환하다가 실패하더라도 
			    컴파일 에러로 간주하지 않고 해당 함수(또는 클래스)를 오버로드 후보군에서 제외시키는 C++의 규칙
			  - C++98/03: SFINAE는 주로 타입(T) 기반 함수 템플릿 특수화에만 한정되어 있음
				(타입 치환 실패 시 후보에서 제외)

			  - 활용
			    + 조건부 함수 오버로드, 타입 특성 분기, 컴파일타임 API 제한, 템플릿 타입 특화 등

			  [예시]
				- 특정 타입만 함수 호출 가능하게 제한
				- 특정 연산(멤버 함수/멤버 변수) 존재 여부에 따라 함수 분기

			  참고: https://en.cppreference.com/w/cpp/language/sfinae
		*/
		{
			int a = 10;
			double b = 3.14;

			print_impl(a);   // 일반 타입
			print_impl(&a);  // 포인터 타입
			print_impl(b); // double 타입은 통과 (SFINAE는 구조화 안됨)
			/*
			출력:
				[generic] Value: 10
				[pointer] Addr: 0x7ffee24813b8, Value: 10		
			*/
		}
	}

	//================================================================================================

	void Test()
	{
		SFINAE_what();
	}

}//SFINAE