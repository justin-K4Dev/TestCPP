#include "stdafx.h"



namespace Pointers
{
	void pointers()
	{
		/*
			📚 포인터 (Pointers)

			변수는 메모리 어딘가에 저장된다.
			그리고 그 메모리 위치에는 "주소(address)" 가 있다.

			보통 우리는 변수 이름으로 값을 사용한다.

				int x = 10;

			하지만 C++에서는
			그 변수의 "주소" 자체를 다룰 수도 있다.
			이 주소를 저장하는 변수가 바로 포인터(pointer)이다.

			즉:
			- 일반 변수: 값을 저장
			- 포인터 변수: 주소를 저장

			포인터를 쓰는 이유:
			1) 다른 변수의 메모리 위치를 직접 다루기 위해
			2) 배열 / 문자열 / 동적 메모리와 연결되기 위해
			3) 함수 인자로 주소를 넘겨 원본 값을 수정하기 위해
			4) 자료구조(리스트, 트리 등)를 만들기 위해
		*/

		{
			int value = 10;
			int* ptr = &value;

			std::cout << "value = " << value << std::endl;
			std::cout << "&value = " << &value << std::endl;
			std::cout << "ptr = " << ptr << std::endl;
			std::cout << "*ptr = " << *ptr << std::endl;
			std::cout << std::endl;

			/*
				설명:
				ptr 은 value 의 주소를 저장하고 있다.
				따라서 *ptr 은 value 자체를 가리킨다.
			*/
		}

		system("pause");
	}

	void address_of_operator()
	{
		/*
			📚 주소 연산자 (Address-of operator)
			
			(&)

			변수 앞에 & 를 붙이면
			그 변수의 메모리 주소를 얻을 수 있다.

			예:
				int myvar = 25;
				int* foo = &myvar;

			여기서 &myvar 는
			"myvar 의 값"이 아니라
			"myvar 가 저장된 주소"이다.
		*/

		{
			int myvar = 25;
			int* foo = &myvar;
			int bar = myvar;

			std::cout << "myvar = " << myvar << std::endl;
			std::cout << "&myvar = " << &myvar << std::endl;
			std::cout << "foo = " << foo << std::endl;
			std::cout << "bar = " << bar << std::endl;
			std::cout << std::endl;

			/*
				설명:
				- myvar 는 값 25
				- &myvar 는 myvar 의 주소
				- foo 는 그 주소를 저장한 포인터
				- bar 는 myvar 값을 그대로 복사한 일반 변수
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 값과 주소는 다르다" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 100;

			std::cout << "x  = " << x << std::endl;
			std::cout << "&x = " << &x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				x 는 저장된 값
				&x 는 x 가 저장된 위치(주소)
				즉 둘은 전혀 다른 의미이다.
			*/
		}

		system("pause");
	}


	void dereference_operator()
	{
		/*
			📚 역참조 연산자 (Dereference operator)

			(*) 

			포인터는 주소를 저장한다.
			그 주소가 가리키는 실제 값을 얻으려면
			포인터 앞에 * 를 붙인다.

			예:
				int x = 25;
				int* p = &x;

				*p  -> x 의 실제 값

			즉 *p 는
			"p 가 가리키는 곳에 들어 있는 값"이라는 뜻이다.
		*/

		{
			int myvar = 25;
			int* foo = &myvar;
			int baz = *foo;

			std::cout << "myvar = " << myvar << std::endl;
			std::cout << "foo   = " << foo << std::endl;
			std::cout << "*foo  = " << *foo << std::endl;
			std::cout << "baz   = " << baz << std::endl;
			std::cout << std::endl;

			/*
				설명:
				foo 는 주소
				*foo 는 그 주소에 저장된 값
				따라서 baz 는 25가 된다.
			*/
		}

		{
			int value = 10;
			int* ptr = &value;

			*ptr = 99;

			std::cout << "value = " << value << std::endl;
			std::cout << "*ptr  = " << *ptr << std::endl;
			std::cout << std::endl;

			/*
				설명:
				*ptr = 99;
				는 ptr 이 가리키는 실제 변수 value 를 수정한 것이다.
			*/
		}

		system("pause");
	}


	void declaring_pointers()
	{
		/*
			📚 포인터 선언 (Declaring pointers)

			포인터는 "어떤 타입을 가리키는지"가 중요하다.
			그래서 선언할 때 가리킬 타입을 함께 적는다.

			문법:
				type* name;

			예:
				int* number;
				char* character;
				double* decimals;

			의미:
			- int*    : int 를 가리키는 포인터
			- char*   : char 를 가리키는 포인터
			- double* : double 를 가리키는 포인터
		*/

		{
			int firstvalue, secondvalue;
			int* mypointer;

			mypointer = &firstvalue;
			*mypointer = 10;

			mypointer = &secondvalue;
			*mypointer = 20;

			std::cout << "firstvalue is " << firstvalue << std::endl;
			std::cout << "secondvalue is " << secondvalue << std::endl;
			std::cout << std::endl;

			/*
				출력:
					firstvalue is 10
					secondvalue is 20

				설명:
				같은 포인터도 실행 중 다른 변수를 가리키도록 바꿀 수 있다.
			*/
		}

		{
			int firstvalue = 5, secondvalue = 15;
			int* p1, * p2;

			p1 = &firstvalue;
			p2 = &secondvalue;
			*p1 = 10;
			*p2 = *p1;
			p1 = p2;
			*p1 = 20;

			std::cout << "firstvalue is " << firstvalue << std::endl;
			std::cout << "secondvalue is " << secondvalue << std::endl;
			std::cout << std::endl;

			/*
				출력:
					firstvalue is 10
					secondvalue is 20
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 선언에서 * 의 의미" << std::endl;
			std::cout << "============================================" << std::endl;

			int* p1, * p2;
			// int* p1, p2;  // p1만 포인터, p2는 int

			std::cout << "포인터 여러 개 선언 시 변수마다 * 를 붙이는 습관이 중요하다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void pointers_n_arrays()
	{
		/*
			📚 포인터와 배열 (Pointers and arrays)

			배열 이름은 많은 상황에서
			첫 번째 원소를 가리키는 포인터처럼 동작한다.

			예:
				int numbers[5];
				int* p = numbers;

			여기서 numbers 는
			&numbers[0] 과 비슷하게 사용된다.

			즉:
				numbers[0]
			와
				*p
			는 같은 첫 번째 원소를 뜻할 수 있다.
		*/

		{
			int numbers[5];
			int* p;

			p = numbers;       *p = 10;
			p++;               *p = 20;
			p = &numbers[2];   *p = 30;
			p = numbers + 3;   *p = 40;
			p = numbers;       *(p + 4) = 50;

			for (int n = 0; n < 5; n++)
				std::cout << numbers[n] << ", ";
			std::cout << std::endl << std::endl;

			/*
				출력:
					10, 20, 30, 40, 50,
			*/
		}

		{
			int arr[5] = { 1, 2, 3, 4, 5 };

			std::cout << "arr[0]    = " << arr[0] << std::endl;
			std::cout << "*arr      = " << *arr << std::endl;
			std::cout << "arr[2]    = " << arr[2] << std::endl;
			std::cout << "*(arr+2)  = " << *(arr + 2) << std::endl;
			std::cout << std::endl;

			/*
				설명:
					arr[2]
				와
					*(arr + 2)
				는 같은 의미이다.
			*/
		}

		system("pause");
	}


	void pointer_initialization()
	{
		/*
			📚 포인터 초기화 (Pointer initialization)

			포인터는 선언과 동시에
			어떤 주소를 가리키도록 초기화할 수 있다.

			예:
				int myvar;
				int* myptr = &myvar;

			중요:
			초기화되는 것은 "포인터가 저장하는 주소"이다.
			가리키는 값이 자동으로 바뀌는 것이 아니다.
		*/

		{
			int myvar = 123;
			int* myptr = &myvar;
			int* another = myptr;

			std::cout << "myvar   = " << myvar << std::endl;
			std::cout << "myptr   = " << myptr << std::endl;
			std::cout << "*myptr  = " << *myptr << std::endl;
			std::cout << "another = " << another << std::endl;
			std::cout << "*another= " << *another << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}


	void pointer_arithmetics()
	{
		/*
			📚 포인터 연산 (Pointer arithmetics)

			포인터는 정수처럼 완전히 자유롭게 계산하는 것이 아니라,
			주로 +, -, ++, -- 만 의미 있게 사용한다.

			그리고 포인터에 1을 더하면
			"다음 원소"를 가리키도록 이동한다.

			중요:
			이동 크기는 1바이트가 아니라
			가리키는 타입의 크기만큼이다.

			예:
				char*  -> +1 하면 1바이트 이동
				int*   -> +1 하면 sizeof(int) 바이트 이동
				long*  -> +1 하면 sizeof(long) 바이트 이동
		*/

		{
			int values[5] = { 10, 20, 30, 40, 50 };
			int* p = values;

			std::cout << "*p      = " << *p << std::endl;
			std::cout << "*p++    = " << *p++ << std::endl;
			std::cout << "*p      = " << *p << std::endl;
			std::cout << "*++p    = " << *++p << std::endl;
			std::cout << std::endl;

			/*
				설명:
				*p++  -> *(p++) 와 같음
						 현재 값 사용 후 p 증가
				*++p  -> *(++p) 와 같음
						 먼저 p 증가 후 새 위치 값 사용
			*/
		}

		{
			int values[3] = { 1, 2, 3 };
			int* p = values;

			++*p;      // values[0] 증가
			(*p)++;    // values[0] 후위 증가

			for (int i = 0; i < 3; ++i)
				std::cout << values[i] << ' ';
			std::cout << std::endl << std::endl;

			/*
				설명:
				++*p   -> ++(*p)
				(*p)++ -> 값 자체를 증가
				둘 다 포인터가 가리키는 "값"을 바꾼다.
			*/
		}

		system("pause");
	}


	// Pointers and const example
	void increment_all(int* start, int* stop)
	{
		int *current = start;
		while (current != stop) {
			++(*current);  // increment value pointed
			++current;     // increment pointer
		}
	}

	void print_all(const int* start, const int* stop)
	{
		const int *current = start;
		while (current != stop) {
			std::cout << *current << '\n';
			++current;     // increment pointer
		}
	}

	void pointers_n_const()
	{
		/*
			📚 포인터와 const (Pointers and const)

			const 와 포인터가 결합되면
			"포인터 자체를 못 바꾸는지"
			또는
			"가리키는 값을 못 바꾸는지"
			를 구분해야 한다.

			대표 4가지:

				int* p1;
					-> non-const pointer to non-const int

				const int* p2;
					-> non-const pointer to const int

				int* const p3;
					-> const pointer to non-const int

				const int* const p4;
					-> const pointer to const int
		*/

		{
			int numbers[] = { 10, 20, 30 };

			increment_all(numbers, numbers + 3);
			print_all(numbers, numbers + 3);
			std::cout << std::endl;

			/*
				출력:
					11
					21
					31
			*/
		}

		{
			int x = 10;
			int y = 20;

			const int* p2 = &x;   // 값 수정 불가, 주소 변경 가능
			int* const p3 = &x;   // 값 수정 가능, 주소 변경 불가

			// *p2 = 30; // 오류
			p2 = &y;     // 가능

			*p3 = 99;    // 가능
			// p3 = &y;  // 오류

			std::cout << "x = " << x << std::endl;
			std::cout << "*p2 = " << *p2 << std::endl;
			std::cout << "*p3 = " << *p3 << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}


	void pointers_n_string_literals()
	{
		/*
			📚 포인터와 문자열 리터럴 (Pointers and string literals)

			문자열 리터럴은
			null 종료된 문자 배열처럼 동작한다.

			예:
				const char* foo = "hello";

			여기서 foo 는
			문자열 리터럴의 첫 글자를 가리키는 포인터이다.

			중요:
			문자열 리터럴은 수정하면 안 되므로
			보통 const char* 로 받는다.
		*/

		{
			const char* foo = "hello";

			std::cout << "foo    = " << foo << std::endl;
			std::cout << "foo[0] = " << foo[0] << std::endl;
			std::cout << "foo[4] = " << foo[4] << std::endl;
			std::cout << "*(foo+4) = " << *(foo + 4) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				foo[4] 와 *(foo+4) 는 둘 다 'o' 이다.
			*/
		}

		system("pause");
	}


	void pointers_to_pointers()
	{
		/*
			📚 포인터를 가리키는 포인터 (Pointers to pointers)

			포인터도 변수이므로 주소를 가진다.
			따라서 포인터를 가리키는 포인터도 만들 수 있다.

			예:
				char a;
				char* b;
				char** c;

			의미:
				a   -> 실제 char 값
				b   -> a를 가리키는 포인터
				c   -> b를 가리키는 포인터
		*/

		{
			char a = 'z';
			char* b = &a;
			char** c = &b;

			std::cout << "a   = " << a << std::endl;
			std::cout << "*b  = " << *b << std::endl;
			std::cout << "**c = " << **c << std::endl;
			std::cout << std::endl;

			/*
				설명:
				c   : char**
				*c  : char*
				**c : char
			*/
		}

		system("pause");
	}


	// Pointers example
	void increase(void* data, int psize)
	{
		if (psize == sizeof(char))
		{
			char* pchar; 
			pchar = (char*)data;
			++(*pchar);
		}
		else if (psize == sizeof(int))
		{
			int* pint;
			pint = (int*)data;
			++(*pint);
		}
	}

	void void_pointers()
	{
		/*
			📚 void 포인터 (void*)

			void* 는 "타입이 정해지지 않은 포인터"이다.

			즉 어떤 주소든 담을 수 있지만,
			가리키는 타입 정보가 없으므로
			직접 역참조할 수는 없다.

			반드시 실제 타입으로 캐스팅한 뒤 써야 한다.
		*/

		{
			char a = 'x';
			int b = 1602;

			increase(&a, sizeof(a));
			increase(&b, sizeof(b));

			std::cout << a << ", " << b << std::endl;
			std::cout << std::endl;

			/*
				출력:
					y, 1603
			*/
		}

		system("pause");
	}


	void invalid_pointers_n_null_pointers()
	{
		/*
			📚 잘못된 포인터와 null 포인터
			(Invalid pointers and null pointers)

			포인터는 주소를 저장하지만,
			그 주소가 항상 유효한 것은 아니다.

			대표적인 위험:
			1) 초기화되지 않은 포인터
			2) 배열 범위를 벗어난 포인터
			3) 이미 해제된 메모리를 가리키는 포인터

			null 포인터는
			명시적으로 "아무 것도 가리키지 않는다"를 뜻하는 값이다.

			현대 C++에서는 보통 nullptr 를 쓴다.
		*/

		{
			int* p = nullptr;
			int* q = 0;

			if (p == nullptr)
				std::cout << "p is null" << std::endl;
			if (q == nullptr)
				std::cout << "q is null" << std::endl;

			std::cout << std::endl;
		}

		{
			int value = 10;
			int* ptr = &value;

			std::cout << "*ptr = " << *ptr << std::endl;

			ptr = nullptr;
			if (ptr == nullptr)
				std::cout << "ptr now points to nowhere" << std::endl;

			std::cout << std::endl;
		}

		/*
			주의:
			null 포인터는 역참조하면 안 된다.

				*ptr   // ptr이 nullptr 이면 매우 위험
		*/

		system("pause");
	}


	// Pointers to functions example
	int addition(int a, int b)
	{
		return (a + b);
	}

	int subtraction(int a, int b)
	{
		return (a - b);
	}

	int operation(int x, int y, int(*functocall)(int, int))
	{
		int g;
		g = (*functocall)(x, y);
		return (g);
	}

	struct PA
	{
		static void call(int v1, int v2)
		{
			std::cout << v1 << ", " << v2 << std::endl;
		}

		void update(int v)
		{
			std::cout << v << std::endl;
		}
	};

	void print(int v, PA* pObj, void(PA::* functocall)(int))
	{
		(pObj->*functocall)(v);
	}


	void pointers_to_functions()
	{
		/*
			📚 함수 포인터 (Pointers to functions)

			C++에서는 함수의 주소를 저장하는 포인터도 만들 수 있다.

			대표 용도:
			- 함수를 다른 함수에 인자로 넘기기
			- 콜백(callback)
			- 테이블 기반 분기 처리

			문법:
				반환형 (*포인터이름)(매개변수...)

			예:
				int(*fp)(int, int) = addition;
		*/

		{
			int m, n;

			int(*minus)(int, int) = subtraction;

			m = operation(7, 5, addition);
			n = operation(20, m, minus);

			std::cout << n << std::endl;
			std::cout << std::endl;

			/*
				출력:
					8

				설명:
				m = 7 + 5 = 12
				n = 20 - 12 = 8
			*/
		}

		{
			void(*static_func)(int, int) = &PA::call;
			void(PA:: * member_func)(int) = &PA::update;

			PA a;

			(*static_func)(100, 10);
			(a.*member_func)(10);

			print(500, &a, member_func);
			std::cout << std::endl;

			/*
				출력:
					100, 10
					10
					500

				설명:
				- static 멤버 함수는 일반 함수 포인터처럼 다룰 수 있다
				- 일반 멤버 함수 포인터는 객체와 함께 호출해야 한다
			*/
		}

		system("pause");
	}


	void Test()
	{
		//pointers();

		//address_of_operator();

		//dereference_operator();

		//declaring_pointers();

		//pointers_n_arrays();

		//pointer_initialization();

		//pointer_arithmetics();

		//pointers_n_const();

		//pointers_n_string_literals();

		//pointers_to_pointers();

		//void_pointers();

		//invalid_pointers_n_null_pointers();

		//pointers_to_functions();
	}

}// end of Pointers