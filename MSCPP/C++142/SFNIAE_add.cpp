#include "stdafx.h"


namespace SFINIAE_AddFeatures
{
	template<typename T>
	void PrintInfo(const T& v) {
		if constexpr (std::is_integral_v<T>) {
			std::cout << "정수: " << v << std::endl;
		}
		else if constexpr (std::is_floating_point_v<T>) {
			std::cout << "실수: " << v << std::endl;
		}
		else if constexpr (std::is_same_v<T, std::string>) {
			std::cout << "문자열: " << v << std::endl;
		}
		else {
			std::cout << "알 수 없는 타입" << std::endl;
		}
	}

	void if_constexpr_use()
	{
		/*
			📚 if constexpr

			  - C++17에서 도입된 컴파일 타임 조건 분기문
			  - 일반 if문과 문법은 같지만, "조건이 false면 해당 분기 코드는 아예 컴파일 대상에서 제외"
			  - 주로 템플릿 함수 내에서 타입이나 컴파일 타임 상수에 따라 다른 코드를 작성할 때 사용
			  - SFINAE, enable_if 등을 대체해 더 직관적이고 안전한 템플릿 분기 구현 가능
			  - "static if"라고도 불림 (런타임 분기 아님)
			  - 헤더 필요 없음, C++17 이상 지원

			  * 주요 효과:
				- 조건이 false인 분기의 코드가 문법적으로 맞지 않아도 무방 (아예 생성 안 됨)
				- 타입/상수 조건별로 전혀 다른 코드(인터페이스) 작성 가능
		*/
		{
			PrintInfo(10);            // 출력: 정수: 10
			PrintInfo(3.14);          // 출력: 실수: 3.14
			PrintInfo(std::string("hi")); // 출력: 문자열: hi
			PrintInfo('A');           // 출력: 정수: 65 (char도 정수 타입)
		}

		system("pause");
	}

	//=============================================================================================

	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, void>
		only_for_integral(T v) {
		std::cout << "정수 전용: " << v << std::endl; 
	}

	void VariableTemplate()
	{
		/*
			📚 C++17 변수 템플릿

			  - std::is_same_v<T, U> : 타입 비교 결과를 value 대신 더 간결하게 사용
			  - std::enable_if_t<B, T> : 조건이 true일 때만 타입 정의(에일리어스 템플릿), 코드 간결
		*/
		{
			only_for_integral(123);         // 정수 전용: 123
			// only_for_integral(3.14);     // 컴파일 에러 (정수가 아님)
		}

		system("pause");
	}

	//=============================================================================================

	template<typename, typename = std::void_t<>>
	struct has_foo : std::false_type {};

	template<typename T>
	struct has_foo<T, std::void_t<decltype(std::declval<T>().foo())>> : std::true_type {};

	// 테스트용 struct
	struct A { void foo() {} };
	struct B {};

	void void_t_use()
	{
		/*
			📚 std::void_t()

			  - 멤버 존재, 표현식 유효성 등 SFINAE 친화적 패턴
		*/
		{
			std::cout << std::boolalpha;
			std::cout << "A에 foo() 있음? " << has_foo<A>::value << std::endl; // A에 foo() 있음? true
			std::cout << "B에 foo() 있음? " << has_foo<B>::value << std::endl; // B에 foo() 있음? false
		}

		system("pause");
	}

	//=============================================================================================

	// C++14/17 이후 에일리어스 템플릿 활용 (짧다!)
	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, void>
		foo(T v) {
		std::cout << "정수 타입: " << v << std::endl;
	}

	template<typename T>
	std::enable_if_t<std::is_floating_point_v<T>, void>
		foo(T v) {
		std::cout << "실수 타입: " << v << std::endl;
	}

	// 테스트용 struct
	struct C { void foo() {} };
	struct D {};

	void AliasTemplate()
	{
		/*
			📚 alias template(에일리어스 템플릿) 개요 및 SFINAE 예제

			  - 기존의 std::enable_if<B, T>::type 대신 std::enable_if_t<B, T>로 코드가 간결해짐
			  - is_same_t, remove_cv_t, add_const_t 등 타입 변환 계열도 모두 동일
			  - 주로 함수 반환형, 템플릿 매개변수 제한 등에 사용
		*/
		{
			foo(42);     // 출력: 정수 타입: 42
			foo(3.14);   // 출력: 실수 타입: 3.14
			// foo("hi"); // 컴파일 에러! (정수/실수 아닌 타입은 SFINAE로 오버로드 집합에서 제외)
		}

		system("pause");
	}

	//=============================================================================================

	// is_convertible_v : 특정 타입만 함수 허용, 변환 가능한 경우만 동작
	template<typename From, typename To>
	void PrintConvertible(const From& v)
	{
		if constexpr (std::is_convertible_v<From, To>)
		{
			To value = v; // 암시적 변환 가능하므로 안전
			std::cout << "변환 가능: " << value << std::endl;
		}
		else
		{
			std::cout << "변환 불가!" << std::endl;
		}
	}

	void std_is_convertible_v_use()
	{
		/*
			📚 std::is_convertible_v

			  - 템플릿 함수에서 "특정 타입으로 변환 가능한가"를 컴파일타임에 체크
			  - 변환 가능할 때만 동작(분기), 불가능하면 안전하게 차단
			  - 타입 기반 유연성 & 안전성 ↑

			  ※ 주요 패턴:
			    1) 값/객체의 자동 변환(예: int → double, Derived* → Base*)
			    2) 템플릿에서 타입 제약, 변환 조건부 분기 등에 사용
		*/
		{
			PrintConvertible<int, double>(42);         // 변환 가능: 42
			PrintConvertible<double, int>(3.14);       // 변환 가능: 3
			PrintConvertible<std::string, const char*>("hi"); // 변환 불가!
			PrintConvertible<const char*, std::string>("hello"); // 변환 가능: hello
			PrintConvertible<float, int>(2.7f);        // 변환 가능: 2
			PrintConvertible<int*, void*>((int*)1234); // 변환 가능: 0x4d2 (포인터 주소)
			PrintConvertible<void*, int*>((void*)1234);// 변환 가능: 0x4d2 (포인터 주소)
			PrintConvertible<std::string, int>("aaa"); // 변환 불가!
			/*
				PrintConvertible<int, double>(42);
				→ int에서 double로 암시적 변환 가능 → 변환 가능: 42

				PrintConvertible<double, int>(3.14);
				→ double에서 int로 암시적 변환 가능 → 변환 가능: 3 (int로 변환되며 소수점 절삭)

				PrintConvertible<std::string, const char*>("hi");
				→ std::string에서 const char*로 암시적 변환 불가 → 변환 불가!

				PrintConvertible<const char*, std::string>("hello");
				→ const char*에서 std::string으로 변환 가능 (생성자 있음) → 변환 가능: hello

				PrintConvertible<float, int>(2.7f);
				→ float에서 int로 암시적 변환 가능 → 변환 가능: 2 (소수점 절삭)

				PrintConvertible<int*, void*>((int*)1234);
				→ int*에서 void*로 암시적 변환 가능 → 변환 가능: 0x4d2
				(0x4d2는 1234의 16진수. 실제 실행 환경에 따라 주소값 형식이 다를 수 있음)

				PrintConvertible<void*, int*>((void*)1234);
				→ void*에서 int*로 암시적 변환 가능 → 변환 가능: 0x4d2

				PrintConvertible<std::string, int>("aaa");
				→ std::string에서 int로 암시적 변환 불가 → 변환 불가!
			*/
		}

		system("pause");
	}

	//=============================================================================================

	void SFINIAE_AddFeatures()
	{
		/*
			📚  SFINAE 지원 추가 기능 (C++17~)

			  - if constexpr : 템플릿/함수 내에서 타입·상수별로 컴파일타임 분기 가능 (SFINAE 대체/보완)
			  - 변수 템플릿(_v suffix): std::is_same_v, std::enable_if_t 등
			  - fold expressions: parameter pack 집계 간편화
			  - std::void_t : 멤버 존재 SFINAE 패턴을 매우 직관적으로 구현
			  - alias templates: enable_if_t 등 코드 간결화
			  - is_convertible_v : 타입 간 암시적 변환 가능 여부를 컴파일타임에 판별하여 분기/제약 조건에 활용
		*/

		std_is_convertible_v_use();

		if_constexpr_use();

		VariableTemplate();

		void_t_use();

		AliasTemplate();
	}

	//=============================================================================================

    void Test()
    {
		SFINIAE_AddFeatures();
    }

}//SFINIAE_AddFeatures