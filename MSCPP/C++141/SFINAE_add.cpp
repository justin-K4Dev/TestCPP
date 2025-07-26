#include "stdafx.h"


#include <type_traits>


namespace SFINAE_AddFeature
{
	// 이 오버로드는 항상 오버로드 집합에 포함됩니다.
	// ... (ellipsis) 파라미터는 오버로드 해석 시 우선순위가 가장 낮습니다.
	void Func(...)
	{
		std::cout << "Catch-all overload called\n";
	}

	// 만약 C가 클래스에 대한 참조 타입(reference-to-class type)이고,
	// F가 멤버 함수 포인터라면, 이 오버로드가 오버로드 집합에 추가됩니다.
	template<class C, class F>
	auto Func(C c, F f) -> decltype((void)(c.*f)(), void())
	{
		std::cout << "Reference overload called\n";
	}

	// 만약 C가 클래스에 대한 포인터 타입(pointer-to-class type)이고,
	// F가 멤버 함수 포인터라면, 이 오버로드가 오버로드 집합에 추가됩니다.
	template<class C, class F>
	auto Func(C c, F f) -> decltype((void)((c->*f)()), void())
	{
		std::cout << "Pointer overload called\n";
	}

	struct X { void f() {} };


	// 체크 함수
	template<typename T>
	class Checker
	{
	public:
		typedef short yes;
		typedef char no;

		template <typename C>
		static yes Func(decltype(&C::Call)) {
			return 1;
		}

		template <typename C>
		static no Func(...) {
			return '0';
		}

		enum { IsFunc = sizeof(Func<T>(0)) == sizeof(yes) };
	};

	class A
	{
	public:
		virtual void Call() { return; }
	};

	class B
	{
	public:
	};

	void SFINAE_AddFeature()
	{
		/*
			📚 SFINAE (Substitution Failure Is Not An Error) 추가 기능

			- C++14/17:
				• constexpr if 등과 조합하면 더욱 직관적이고 안전한 오버로드/멤버 탐지, 분기가 가능
				• 라이브러리 수준에서 다양한 "expression SFINAE" 기반 오버로드 구현이 보편화
				• (예: decltype((c.*f)()), decltype((c->*f)()), decltype(&C::Call) 등)
				• C++11 이후에는 "SFINAE-friendly" 코드가 더 간결해지고, C++20에서는 Concepts로 대체/보완

			- 실전 활용:
				• decltype과 SFINAE로 "특정 멤버/연산이 가능한 타입에 대해서만 함수가 활성화"
				• 오버로드/템플릿 선택, 멤버 존재 탐지(Checker 클래스 예시), 안전한 API 분기 등

			- Visual Studio 2015/2017~: expression SFINAE, decltype, auto 기반 오버로드 해석의 표준 호환성, 버그 개선 등 대폭 강화됨

			참고: http://en.cppreference.com/w/cpp/language/sfinae
		*/
		{
			X x;

			Func(x, &X::f);
			Func(&x, &X::f);
			Func(42, 1337);

			system("pause");

			/*
			출력:
				Reference overload called
				Pointer overload called
				Catch-all overload called
			*/
		}

		/*
			decltype과 SFINAE를 활용하여 컴파일 타임에(컴파일 시점에) 해당 요소(멤버, 함수 등)가 존재하는지 확인한다.
		*/
		{
			std::cout << Checker<A>::IsFunc << ", " << Checker<B>::IsFunc << std::endl;

			system("pause");
			/*
			출력:
				1, 0
			*/
		}
	}


	void Test()
	{
		SFINAE_AddFeature();
	}
}