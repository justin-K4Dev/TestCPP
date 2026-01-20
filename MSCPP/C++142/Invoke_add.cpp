#include "stdafx.h"


namespace Invoke_AddFeatures
{
	//=============================================================================================

	int free_add(int a, int b) { return a + b; }

	struct Foo {
		int x = 10;

		int add(int v) const { return x + v; }
		void set(int v) { x = v; }
	};

	void invoke_what()
	{
		/*
			📚 std::invoke

			  - C++17에서 도입
			  - 함수 “호출(call)을 한 가지 규칙으로 통일”해주는 유틸
			  - std::reference_wrapper도 처리해줘서 std::ref(x)로 감싼 것도 자연스럽게 작동 !!!

			  1) 일반 호출 가능(callable)
			    - f(args...)
			  2) 멤버 함수 포인터 호출
			    - pmf가 멤버함수 포인터일 때
                  std::invoke(pmf, obj, args...) → (obj.*pmf)(args...)
                  std::invoke(pmf, ptr, args...) → (ptr->*pmf)(args...)
			  3) 멤버 데이터 포인터 접근
			    - pmd가 멤버데이터 포인터일 때
				  std::invoke(pmd, obj) → obj.*pmd
                  std::invoke(pmd, ptr) → ptr->*pmd			  
		*/

		// 1) 일반 함수
		std::cout << std::invoke(&free_add, 3, 4) << "\n"; // 7

		// 2) 람다/펑터
		auto lam = [](std::string s) { return s.size(); };
		std::cout << std::invoke(lam, std::string("hello")) << "\n"; // 5

		// 3) 멤버 함수 포인터
		Foo f;
		auto pmf = &Foo::add;
		std::cout << std::invoke(pmf, f, 5) << "\n";   // (f.*pmf)(5) => 10+5=15
		std::cout << std::invoke(pmf, &f, 7) << "\n";  // (f->*pmf)(7)=> 10+7=17

		// 4) 멤버 데이터 포인터
		auto pmd = &Foo::x;
		std::cout << std::invoke(pmd, f) << "\n";   // f.x => 10
		std::invoke(pmd, f) = 99;                   // f.x = 99 처럼 쓸 수 있음(레퍼런스 반환)
		std::cout << f.x << "\n";                   // 99

		// 5) reference_wrapper도 자연스럽게 지원
		Foo g;
		auto pset = &Foo::set;
		std::invoke(pset, std::ref(g), 123);        // (g.*pset)(123)
		std::cout << g.x << "\n";                   // 123
	}

	//=============================================================================================

	struct Woo {
		int data = 7;

		int add(int x) const { return data + x; }
		void set(int x) { data = x; }
	};

	int free_func(double) { return 123; }

	void invoke_result_t_what()
	{
		/*
			📚 std::invoke_result_t

			  - C++17에서 도입
			  - std::invoke(std::declval<F>(), std::declval<Args>()...) 를 호출했을 때의 “반환 타입”
			    을 컴파일 타임에 계산해서 타입으로 돌려주는 타입 트레잇
			  - F는 함수/람다/펑터/멤버함수포인터/멤버데이터포인터 등 “호출 가능한(callable)” 것
			  - Args... 는 호출에 쓰이는 인자 타입들
			  - 결과는 R 타입 (또는 void)
			  - 즉 “이걸 호출하면 결과 타입이 뭐냐?” 를 자동으로 알아내는 용도

		*/

		// 1) 람다
		auto lam1 = []() { return 10; };
		static_assert(std::is_same_v<std::invoke_result_t<decltype(lam1)>, int>);

		auto lam2 = [](int a, int b) -> long long { return (long long)a + b; };
		static_assert(std::is_same_v<std::invoke_result_t<decltype(lam2), int, int>, long long>);

		auto lam3 = []() { /* void */ };
		static_assert(std::is_same_v<std::invoke_result_t<decltype(lam3)>, void>);

		// 2) 일반 함수 포인터 / 함수
		static_assert(std::is_same_v<std::invoke_result_t<decltype(&free_func), double>, int>);

		// 3) 멤버 함수 포인터
		using PMF = int (Woo::*)(int) const;
		PMF pmf = &Woo::add;

		// Woo 객체로 호출
		static_assert(std::is_same_v<std::invoke_result_t<PMF, Woo, int>, int>);

		// Woo* 포인터로 호출
		static_assert(std::is_same_v<std::invoke_result_t<PMF, Woo*, int>, int>);

		// 4) 멤버 데이터 포인터
		auto pmd = &Woo::data;

		// Woo 객체로 접근
		static_assert(std::is_same_v<std::invoke_result_t<decltype(pmd), Woo&>, int&>);

		// Woo* 포인터로 접근
		static_assert(std::is_same_v<std::invoke_result_t<decltype(pmd), Woo*>, int&>);

		// 5) 실제로 써보기 (런타임 출력)
		Woo f;
		std::cout << std::invoke(pmf, f, 5) << "\n";      // 7 + 5 = 12
		std::cout << std::invoke(pmd, f) << "\n";         // 7
	}

	//=============================================================================================

	void Test()
	{
		invoke_result_t_what();

		invoke_what();
	}

}//Invoke_AddFeatures