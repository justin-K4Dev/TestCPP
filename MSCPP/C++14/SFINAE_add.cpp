#include "stdafx.h"


#include <iostream>
#include <type_traits>
#include <array>
#include <map>


namespace SFINAE_AddFeatures
{
	template<typename T>
	class zero_init
	{
	protected:
		T val;
	public:
		zero_init() : val(static_cast<T>(0)) { }
		zero_init(T val) : val(val) { }
		operator T&() { return val; }
		operator T() const { return val; }
	};

	void implicit_conversions()
	{
		/*
			📚 암시적 변환(Implicit Conversion)과 C++14 switch 조건문

			  - C++11까지:
				· 변환 연산자(operator T() / operator T&())가 여러 개면 switch, case 등에서 모호성(ambiguous conversion)으로 컴파일 오류
				· 예: operator int()와 operator int&()가 모두 있으면, switch(i)에서 어떤 변환을 쓸지 애매하여 오류

			  - C++14부터:
				· 변환 결과 타입이 동일하면(예: 모두 int) 모호성 없이 switch 등에서 사용 가능
				· 즉, operator int(), operator int&() 모두 있어도 switch(i) 가능

			  - 결론:
				· C++14 이전엔 switch, case 등에서 암시적 변환 충돌로 오류
				· C++14 이상에서는 동일 타입 변환이 여러 개 있어도 switch 가능
		*/
		{
			zero_init<int> i;
			assert(i == 0);
			i = 7;
			assert(i == 7);

			// C++11까지는 아래 코드가 컴파일 오류! (multiple user-defined conversions)
			//switch (i) { }     // ❌ C++11에서는 ambiguous error

			// C++14부터는 허용! (둘 다 int로 변환 가능 → switch에서 OK)
			switch (i) { case 7: std::cout << "OK" << std::endl; break; }

			// 아래 코드는 항상 OK (i+0으로 산술 변환)
			switch (i + 0) {
			case 0: std::cout << "i is zero" << std::endl; break;
			case 7: std::cout << "i is seven" << std::endl; break;
			default: std::cout << "other" << std::endl;
			}

			system("pause");
		}
	}

	//================================================================================================

	template <typename T, typename U>
	struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type
	{ };

	void decay_use()
	{
		/*
			📚 std::decay(), decay_equiv()

			  - std::decay<T>는 C++11 표준 라이브러리 type traits의 한 종류로,
			    템플릿 인자 T를 "함수 인자 타입으로 전달될 때의 원시 타입"으로 변환해준다.

			  - 변환 규칙:
				· 참조/const/volatile 속성 제거: int&, const int&, int&& → int
				· 배열 → 포인터: int[2] → int*
				· 함수 타입 → 함수 포인터: int(int) → int(*)(int)
				· (기본적으로 'Value Transformation' 역할)

			  - decay_equiv<T, U>는 std::decay<T>::type과 U가 동일한 타입인지 std::is_same으로 판정하는 헬퍼 타입
				· value가 true면 decay 결과가 U와 같다는 의미

			  - 실전에서는 템플릿 함수/클래스에서 타입 통일, 오버로드 선택, std::tuple, std::function 등에서 많이 사용

			  예시:
			    decay_equiv<int&, int>::value             // true
				decay_equiv<int[2], int*>::value		  // true
				decay_equiv<int(int), int(*)(int)>::value // true
		*/

		//기본적으로 std::decay<T>는 T 타입 본연의 형식으로 strip-down 시키는 역할
		//std::decay는 Implicit conversions 중 Value transformations 역할을 수행
		{
			std::cout << std::boolalpha
				<< decay_equiv<int, int>::value << '\n'
				<< decay_equiv<int&, int>::value << '\n'
				<< decay_equiv<int&&, int>::value << '\n'
				<< decay_equiv<const int&, int>::value << '\n'
				<< decay_equiv<int[2], int*>::value << '\n'
				<< decay_equiv<int(int), int(*)(int)>::value << '\n';

			system("pause");
			/*
			출력:
				true
				true
				true
				true
				true
				true
			*/
		}
	}

	//================================================================================================

	class car;
	class bar;

	template<class T>
	struct check
	{
		// T == bar인 경우에만 이 함수가 활성화됨
		template<class Q = T>
		typename std::enable_if<std::is_same<Q, bar>::value, bool>::type test()
		{
			return true;
		}

		// T != bar인 경우에만 이 함수가 활성화됨
		template<class Q = T>
		typename std::enable_if<!std::is_same<Q, bar>::value, bool>::type test()
		{
			return false;
		}
	};

	void enable_if_use_1()
	{
		/*
			📚 std::enable_if() + std::is_same()를 활용한 SFINAE 조건부 함수 오버로드 (C++11~)

			  - std::enable_if<B, T>는 템플릿 인스턴스화 시 조건(B)이 true면 T, 아니면 substitution failure(후보 함수 제외)
			  - std::is_same<T, U>::value는 T와 U가 동일한 타입인지 컴파일 타임에 판정

			  - 이 예제의 구조:
				· check<T> 구조체 안에 test() 함수가 두 개 (템플릿 오버로드)
				· test()는 T가 bar일 때만 true, 그 외에는 false 반환
				· enable_if를 이용해 타입이 bar일 때와 아닐 때 각각 별도의 test()가 "오버로드 후보"로 동작
				· SFINAE로 인해 타입에 따라 딱 한 쪽만 인스턴스화

			  - 활용 효과:
				· 특정 타입(bar)만 true를 반환하는 API 구현
				· 템플릿 기반 컴파일 타임 타입 분기, 조건부 활성화, 정책 분리 등에 응용
		*/
		{
			check<car> check_car;
			check<bar> check_bar;

			// check_car.test()는 false, check_bar.test()는 true이므로 아래 if문이 실행됨
			if (!check_car.test() && check_bar.test()) {
				std::cout << "It works!" << std::endl;
			}

			system("pause");
			/*
			출력:
				It works!
			*/
		}
	}

	//================================================================================================

	template < typename... > struct are_derived_from;

	template < typename BASE, typename T >
	struct are_derived_from< BASE, T > : std::is_base_of< BASE, typename std::decay<T>::type >
	{};

	template < typename BASE, typename FIRST, typename... REST >
	struct are_derived_from< BASE, FIRST, REST... > : std::conditional< std::is_base_of< BASE, typename std::decay<FIRST>::type >::value
										                             && are_derived_from<BASE, REST... >::value
													                  , std::true_type
		                                                              , std::false_type >::type
	{};

	struct base { /* ... */ };

	template < typename FIRST, typename... REST >
	typename std::enable_if< are_derived_from<base, FIRST, REST... >::value >::type foo(FIRST*, REST*...)
	{
		std::cout << "all are pointers to types derived from base\n";
	}

	template < typename FIRST, typename... REST >
	typename std::enable_if< are_derived_from<base, FIRST, REST... >::value >::type foo(FIRST&&, REST&&...)
	{
		std::cout << "all are references to types derived from base\n";
	}

	void enable_if_use_2()
	{
		/*
			📚 std::enable_if() + std::conditional()를 활용한 SFINAE 조건부 함수 오버로드 (C++11~)

			  - std::is_base_of<Base, T>
		        : T가 Base의 파생 타입인지 컴파일 타임에 판별
			  - std::decay<T>
		        : T를 참조/const/배열/함수 등에서 '본질적 타입'으로 변환
			  - std::conditional<cond, T, F>
		        : cond가 true면 T, false면 F를 타입으로 선택(컴파일 타임 if)
			  - std::true_type, std::false_type
		        : 타입 기반 true/false 판정(struct 상속/분기에서 활용)
			  - std::enable_if<cond, T>
		        : cond가 true일 때만 T 타입 활성화, 아니면 SFINAE로 제외

	          - 패턴: 가변 인자 템플릿/재귀와 조합하면
		        "여러 타입이 모두 Base 파생인지 판별",
		        조건부 함수 활성화(오버로드), 안전한 API 설계, 라이브러리 타입 검증 등에 강력하게 활용

		      ※ 모두 C++11~ 표준 라이브러리에서 제공
		*/
		{
			struct A : base
			{
			} a;
			struct B : base
			{
			} b;
			struct C : base
			{
			} c;
			struct D : base
			{
			} d;

			foo(&a, &b, &c, &d);
			foo(a, b, B(), c, d, D());

			system("pause");
			/*
			출력:
				all are pointers to types derived from base
				all are references to types derived from base
			*/
		}
	}

	//================================================================================================

	template <typename Ref, typename T1, typename... TN>
	struct all_match;

	template <typename Ref, typename T>
	struct all_match<Ref, T>
	{
		static constexpr bool value = std::is_same<T, Ref>::value;
	};

	template <typename Ref, typename T1, typename... TN>
	struct all_match
	{
		static constexpr bool value = std::is_same<T1, Ref>::value && all_match<Ref, TN...>::value;
	};

	template <typename T, typename... U>
	typename std::enable_if<all_match<int, U...>::value, void>::type
	testFunction(T a, U... bs)
	{
		std::cout << "bs are integers\n";
	}

	template <typename T, typename... U>
	typename std::enable_if<all_match<char, U...>::value, void>::type
	testFunction(T a, U... bs)
	{
		std::cout << "bs are chars\n";
	}

	void enable_if_use_3()
	{
		/*
			📚 std::enable_if와 std::is_same()을 활용한 SFINAE 조건부 함수 오버로드 (C++11~)

			  - all_match<Ref, U...>
				: 가변 인자 U...이 모두 Ref 타입과 동일한지 컴파일 타임에 판별
				: std::is_same<T, Ref>::value로 각 타입을 비교, 재귀적 AND 평가

			  - std::enable_if<cond, T>
				: cond가 true일 때만 T 타입 활성화, 아니면 SFINAE로 함수가 후보에서 제외

			  - 패턴: 함수 오버로드 시 가변 인자 타입 집합에 대해 "모두 int"일 때, "모두 char"일 때 등으로 분기 가능
				· testFunction 함수는 bs...가 모두 int면 "bs are integers", 모두 char면 "bs are chars" 출력
				· 조건이 안 맞는 조합(예: int, float, int)은 컴파일 에러

			  ※ 모두 C++11~ 표준 라이브러리에서 제공
		*/
		{
			testFunction(1.0, 3);				//ok
			testFunction(1.0, 'c', 'r', 'p');	//ok
			//testFunction(1.0, 3, 4.0f, 5);	//fail

			system("pause");

			/*
			출력:
				bs are integers
				bs are chars
			*/
		}
	}

	//================================================================================================

	template <typename T, typename = int>
	struct HasX : std::false_type
	{
		bool GetValue() { return this->value; }
	};

	template <typename T>
	struct HasX <T, decltype((void)T::x, 0)> : std::true_type
	{
		bool GetValue() { return this->value; }
	};

	struct A { int x; };
	struct B { int y; };

	void true_type_use()
	{
		/*
			📚 std::true_type, std::false_type, decltype, SFINAE를 활용한 멤버 변수 존재 판정 (C++11~)

			  - HasX<T>
				: T 타입이 정적 멤버 x를 가지고 있으면 std::true_type, 아니면 std::false_type 상속
				: partial specialization과 decltype((void)T::x, 0) 트릭으로 컴파일 타임에 멤버 존재 판별

			  - GetValue()
				: 상속된 value 멤버(true_type/false_type) 값을 런타임 bool로 반환

			  - 활용:
				· 멤버 변수(혹은 함수) 존재 여부에 따라 정책/동작을 컴파일 타임 분기
				· 라이브러리/템플릿에서 API 안전성, 자동 특화 등에 사용

			  ※ 모두 C++11~ 표준 라이브러리에서 제공
		*/
		{
			HasX<A> a;
			std::cout << a.GetValue() << std::endl; // 1 (A는 x 멤버를 가짐)
			HasX<B> b;
			std::cout << b.GetValue() << std::endl; // 0 (B는 x 멤버 없음)

			system("pause");
			/*
			출력:
				1
				0
			*/
		}
	}

	//================================================================================================

	#define IS_MEMBER(T1, M, funcName)																																			\
	struct {																																									\
		struct isMember { bool M; };																																			\
		template<typename T>																																					\
		struct checkMember : isMember, public T																																	\
		{ };																																									\
																																												\
		enum return_t { not_member = 0, is_object = 1, is_function = 3 };																										\
																																												\
        /* 멤버 존재하지 않을 때: not_member(0) 반환 */                                                                                 \
        template<typename T, typename = decltype(checkMember<T>::M)> constexpr return_t what_member() { return not_member;  }            \
        /* 멤버 변수(필드)일 때: is_object(1) 반환 */                                                                                   \
        template<typename T> typename std::enable_if<std::is_member_object_pointer<decltype(&T::M)>::value, return_t>::type constexpr what_member() { return is_object; } \
        /* 멤버 함수일 때: is_function(2) 반환 */                                                                                       \
        template<typename T> typename std::enable_if<std::is_member_function_pointer<decltype(&T::M)>::value, return_t>::type constexpr what_member() { return is_function; } \
        constexpr operator return_t() { return what_member<T1>(); } 																										\
	} funcName

	struct Object
	{
		int id;
		std::string name;

		int GetID() { return id; }
	};

	// 전역/네임스페이스 범위에서만 선언 가능(함수 내 선언 불가)
	IS_MEMBER(Object, id, isIDofObject);		// id가 멤버 변수면 1
	IS_MEMBER(Object, name, isNameofObject);    // name이 멤버 변수면 1
	IS_MEMBER(Object, GetID, isGetIDofObject);  // GetID가 멤버 함수면 2

	//known at compile time
	enum { const_isIDofObject = (int)isIDofObject };
	static constexpr int const_isGetIDofObject = isGetIDofObject;

	void check_type()
	{
		/*
			📚 멤버 변수/함수 존재 및 타입 판정 매크로(IS_MEMBER)와 SFINAE + <type_trait> 활용 (C++11~)

			  - IS_MEMBER(T1, M, funcName):
				· T1 타입의 멤버 M이 "없음/변수/함수" 중 무엇인지 컴파일 타임에 판별
				· SFINAE, decltype, std::is_member_object_pointer, std::is_member_function_pointer 등 type trait 활용
				· 매크로 확장 후 funcName으로 해당 정보(존재 여부, 멤버 종류)에 대한 constexpr 변환 연산자 제공

			  - 사용법:
				· IS_MEMBER(Object, id, isIDofObject);   // id 멤버가 변수면 1, 아니면 0 또는 2
				· IS_MEMBER(Object, GetID, isGetIDofObject); // GetID가 멤버 함수면 2

			  - 활용:
				· 컴파일 타임에 멤버의 존재/종류를 판별해 static_assert, enum, 정책 분기 등에서 활용
				· 라이브러리/템플릿에서 API 자동화, 타입별 특화 등에도 사용
		*/

		// 0 = 멤버 없음, 1 = 멤버 변수, 2 = 멤버 함수
		std::cout << std::boolalpha <<	"0 - not_member, 1 - is_object, 2 - is_function \n\n" <<
										"is id member of Object = " << isIDofObject << std::endl <<
										"is name member of Object = " << isNameofObject << std::endl <<
										"is GetID() member of Object = " << isGetIDofObject << std::endl <<
		std::endl;

		system("pause");
		/*
		출력:
			0 - not_member, 1 - is_object, 2 - is_function

			is id member of Object = 1
			is name member of Object = 1
			is GetID() member of Object = 2
		*/
	}

	//================================================================================================

	void Test()
	{
		check_type();

		true_type_use();

		enable_if_use_3();

		enable_if_use_2();

		enable_if_use_1();

		decay_use();

		implicit_conversions();
	}

}//SFINAE_AddFeatures