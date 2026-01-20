#include "stdafx.h"


namespace Initialization
{
	void default_initialization_what()
	{
		/*
			default-initialization (기본 초기화)

			  - 선언만 하고 값은 안 줌 → 타입에 따라 알아서 처리
		*/
		{
			struct Foo
			{
				int n;
				std::string s;

				// 기본 생성자: 값 안 채움 (일부러 비워둠)
				Foo()
				{
					std::cout << "Foo() 기본 생성자 호출\n";
				}
			};

			int x;				// 로컬 변수 T x;
			Foo f;				// 로컬/멤버 T x;
			auto p = new int;	// new T;

			/*
				로컬 변수
				  - 초기화 안 됨 (쓰레기값, indeterminate value)
			
				void foo()
				{
					int x;   // ⚠ 값이 무엇인지 보장 X (UB 위험), (쓰레기값, indeterminate value)
				}
			*/

			/*
				클래스 타입
				  - 기본 생성자가 있으면 → 그 기본 생성자 호출			 
			
				struct Player
				{
					int hp;
					Player() : hp(100) {}
				};

				void foo()
				{
					Player p; // default-init → Player() 호출, hp == 100
				}
			*/

			/*
				전역/정적 변수
				  - 먼저 zero-initialization 되고 나서 default-init

				int g; // 전역 변수 g는 0으로 보장됨
			*/
		}
	}

	void value_initialization_what()
	{
		/*
			value-initialization (값 초기화)

			  - 타입에 맞는 기본값으로 안전하게 채운다.

		*/
		{
			struct Foo
			{
				int n;
				std::string s;
			};

			int a = int();       // 1) T a = T();
			Foo f = Foo();       // 2) T f = T();
			auto p = new int();  // 3) new T();
			int x{};             // 4) T x{}; (기본형에서 사실상 value-init 효과)
			Foo y{};             // 5) T y{}; (많은 경우 기본 생성자/멤버 기본값)

			/*
				기본형 & 포인터

				int a = int();		→ a == 0
				bool b = bool();	→ b == false
				int* p = new int(); → *p == 0
			*/

			/*
				클래스 & struct
				  - 기본 생성자가 있으면 → 기본 생성자 호출
                    없고 aggregate이면 → 멤버들을 0/기본값으로 채움

				Foo f1 = Foo();     // f1.n == 0, f1.s == ""
				Foo f2{};			// t2.n == 0, t2.s == "" (aggregate + 빈 {} → 비슷한 효과)
			*/
		}
	}

	void list_initialization_what()
	{
		/*
			list-initialization (리스트 초기화, {})

			  - 중괄호 {}로 초기화하는 모든 것
			  - uniform initialization 이라고도 부름 !!!
		*/
		{
			struct Foo
			{
				int n;
				std::string s;
			};

			int a{ 0 };                 // direct-list-init
			int b = { 0 };				// copy-list-init

			Foo f1{ 1, "hi" };          // direct-list-init
			Foo f2 = { 1, "hi" };       // copy-list-init

			Foo f3{};					// 빈 리스트
			std::vector<int> v{ 1,2,3 };

			/*
				T x{arg1, arg2, ...};
				  - initializer_list 생성자 우선 탐색
				  - 없으면 일반 생성자 중에서 최적 후보 선택
				  - 만약 aggregate 타입이면 → aggregate-initialization 규칙 적용

				Foo f1{1, "hi"};  // aggregate init: f1.n=1, f1.s="hi"
				Foo f2{};         // f2.n=0, f2.s=""

				std::vector<int> v{1,2,3}; // v = [1,2,3]
			*/

			/*
				T x{}; (값 없는 빈 {}):
				  - 기본형 → 0 / false / nullptr (value-init 효과)
                  - aggregate struct → 멤버들을 0/기본값으로 채움
				  - 일반 클래스 → 기본 생성자 호출

				int a{0};       // a == 0
				int b{};        // b == 0 (value-init 비슷)
			*/
		}
	}

	void aggregate_initialization_what()
	{
		/*
			aggregate-initialization (리스트 초기화, {})
			  - 생성자 없는 단순 struct/배열을 {}로 멤버 순서대로 채움

			* 조건			
			  - 사용자 정의 생성자 없음
			  - private/protected 멤버 없음
              - 상속/가상 함수 없음
		*/
		{
			struct Vector3
			{
				float x;
				float y;
				float z;
			};

			Vector3 v1{ 1.0f, 2.0f, 3.0f }; // x=1, y=2, z=3, (list-init)
			Vector3 v2{};					// x=0, y=0, z=0 (빈 리스트 → 0으로 채움)
		}
	}


	void Test()
	{
		//aggregate_initialization_what();

		//list_initialization_what();

		//value_initialization_what();

		//default_initialization_what();
	}

}// end of Initialization