#include "stdafx.h"



namespace NSDMI_AddFeature
{
	void NSDMI_AddFeature()
	{
		/*
			📚 NSDMI (Non-Static Data Member Initializers) 추가 기능

			  - C++14부터: 집합체(aggregate) 타입도 NSDMI 지원
				+ 중괄호 초기화로 값이 들어오지 않은 멤버는 NSDMI 값으로 자동 초기화
				+ 즉, 일부 멤버만 중괄호 초기화 값으로 지정하면, 나머지는 선언부의 디폴트값(NSDMI) 사용
			  
			  - 활용: 구조체/배열/집합체의 초기값 지정, 멤버 일부 생략 가능, 실무에서 유연한 값 할당에 매우 유용
			  - 참고: https://en.cppreference.com/w/cpp/language/aggregate_initialization
		*/
		{
			struct Univ
			{
				std::string name;
				int rank;
				std::string city = "unknown";
			};

			Univ u = { "Columbia",10 };
			std::cout << u.name << ' ' << u.rank << ' ' << u.city << '\n';

			system("pause");

			/*
			출력:
				Columbia 10 unknown
			*/
		}
		{
			struct X { int i, j, k = 42; };

			X a[] = { 1, 2, 3, 4, 5, 6 };

			for each(X & v in a) {
				std::cout << v.i << ',' << v.j << ',' << v.k << std::endl;
			}

			system("pause");

			/*
			출력:
				1,2,3
				4,5,6
			*/
		}
		{
			struct A
			{
				struct X { int a, b; };
				X x = { 1, 2 };
				int n;
			};

			A a = { { 10 }, 5 };
			std::cout << a.x.b << std::endl;

			system("pause");

			/*
			출력:
				0
			*/
		}
	}

	void Test()
	{
		NSDMI_AddFeature();
	}
}