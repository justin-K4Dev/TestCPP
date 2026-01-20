#include "stdafx.h"



namespace Constructor
{
	// 생성자/소멸자/연산자 오버로드 default/delete 예제
	class EntityObj {
	public:
		EntityObj() = default;								// 기본 생성자 자동 생성
		EntityObj(const EntityObj&) = default;				// 복사 생성자 자동 생성
		~EntityObj() = default;								// 소멸자 자동 생성
		EntityObj& operator=(const EntityObj&) = default;   // 복사 대입 연산자 자동 생성

		void func(int i) { std::cout << "int: " << i << std::endl; }

		void func(double d) = delete;           // double 인자 금지

		void* operator new(size_t) = delete;    // new 연산자로 객체 생성 금지
	};

	struct EntityObj2 {
		void func(int i) { std::cout << "int: " << i << std::endl; }
		void func(double d) = delete;           // double 인자 금지
	};

	void consturctor()
	{
		/*
			📚 생성자(Constructor), 소멸자(Destructor), default/delete 지정자, 타입 trait
			
			  - 생성자(Constructor): 객체가 생성될 때 자동으로 호출되는 특수 함수.
			  - 소멸자(Destructor): 객체의 생명 주기가 끝날 때 호출되는 함수.
			  - =default: 컴파일러가 기본(자동) 구현을 생성하도록 명시.
			  - =delete: 컴파일러가 해당 함수(생성자, 연산자 등)를 자동으로 생성하지 않게 명시. (금지)
			  - 함수 오버로드 시 타입별로 delete 지정 가능(예: double 인자 금지).
			  - new/delete 연산자도 금지 가능.

			📚 타입 trait과 static_assert

			  - <type_traits> 헤더의 std::is_xxx 시리즈로 타입의 특성을 컴파일 타임에 검사 가능.
			  - static_assert와 조합하면, 특정 조건이 맞지 않으면 컴파일 에러를 강제할 수 있음.
		*/


		// 생성자/소멸자, delete 예제
		{
			EntityObj t;
			t.func(42);          // 정상 출력

			// t.func(3.14);     // 컴파일 에러 (double 금지)
			// Test* p = new Test; // 컴파일 에러 (new 연산자 금지)
		}

		system("pause");
		
		/*
		출력:
			42
		*/
	}

	//=============================================================================================

	// 타입 trait(static_assert) 예제
	template <class T>
	void my_swap(T& a, T& b)
	{
		static_assert( std::is_copy_constructible<T>::value
			         , "Swap requires copying" );
		static_assert(   std::is_nothrow_copy_constructible<T>::value
			          && std::is_nothrow_copy_assignable<T>::value
			         , "Swap may throw" );
		auto c = b;
		b = a;
		a = c;
	}

	template <class T>
	struct data_structure
	{
		static_assert( std::is_default_constructible<T>::value
			         , "Data Structure requires default-constructible elements");
	};

	struct NoCopy {
		NoCopy(const NoCopy&) = default;
		NoCopy() = default;
	};

	struct NoDefault {
		// 복사 생성자 없음
		// 기본 생성자만 존재
		NoDefault() = default;
	};

	void std_is_copy_constructible_use()
	{
		int a = 1, b = 2;
		my_swap(a, b); // int는 복사, 대입 모두 가능 → OK

		NoCopy nc1, nc2;
		my_swap(nc1, nc2); // OK

		data_structure<int> ds_ok; // OK (int는 기본 생성 가능)
		// data_structure<NoCopy> ds_err; // 컴파일 에러 (NoCopy는 기본 생성자 없음)

		system("pause");
	}


	void Test()
	{
		//consturctor();

		//std_is_copy_constructible_use();
	}

}//Constructor