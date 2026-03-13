#include "stdafx.h"



namespace OtherDataTypes
{
	void type_aliases()
	{
		/*
			📚 타입 별칭 (Type aliases)
			(typedef / using)

			타입 별칭은
			기존 타입에 "다른 이름"을 붙이는 기능이다.

			즉 새로운 타입을 만드는 것이 아니라,
			기존 타입의 별명(alias)을 만드는 것이다.

			C++에는 두 가지 문법이 있다.

			1) typedef
				typedef 기존타입 새이름;

			2) using
				using 새이름 = 기존타입;

			예:
				typedef unsigned int WORD;
				using WORD2 = unsigned int;

			둘 다 의미는 같다.
			다만 현대 C++에서는 using 이 더 읽기 쉽고
			템플릿과 함께 사용할 때도 더 유연해서 자주 선호된다.
		*/

		{
			typedef char C;
			typedef unsigned int WORD;
			typedef char* pChar;
			typedef char field[50];

			C mychar = 'A';
			WORD myword = 100;
			char buffer[] = "Hello";
			pChar ptc2 = buffer;
			field name = "Justin";

			std::cout << "mychar = " << mychar << std::endl;
			std::cout << "myword = " << myword << std::endl;
			std::cout << "ptc2   = " << ptc2 << std::endl;
			std::cout << "name   = " << name << std::endl;
			std::cout << std::endl;

			/*
				설명:
				C      -> char
				WORD   -> unsigned int
				pChar  -> char*
				field  -> char[50]

				즉 새 타입을 만든 것이 아니라
				기존 타입에 새 이름을 붙인 것이다.
			*/
		}

		{
			using C = char;
			using WORD = unsigned int;
			using pChar = char*;
			using field = char[50];

			C mychar = 'Z';
			WORD myword = 500;
			char text[] = "World";
			pChar p = text;
			field title = "Matrix";

			std::cout << "mychar = " << mychar << std::endl;
			std::cout << "myword = " << myword << std::endl;
			std::cout << "p      = " << p << std::endl;
			std::cout << "title  = " << title << std::endl;
			std::cout << std::endl;

			/*
				설명:
				using 도 typedef 와 같은 역할을 한다.
				현대 C++에서는 보통 using 을 더 많이 본다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 타입 별칭은 새 타입이 아니다" << std::endl;
			std::cout << "============================================" << std::endl;

			using WORD = unsigned int;

			WORD a = 10;
			unsigned int b = 20;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "WORD 는 unsigned int 의 별칭일 뿐이다." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				WORD 와 unsigned int 는 본질적으로 같은 타입이다.
				즉 완전히 새로운 독립 타입이 아니다.
			*/
		}

		/*
			실무 팁:
			타입 별칭은
			- 너무 긴 타입 이름을 줄이거나
			- 의미를 더 분명히 하거나
			- 나중에 실제 타입 변경을 쉽게 만들 때 유용하다.
		*/

		system("pause");
	}


	void unions()
	{
		/*
			📚 공용체 (Unions)

			union 은
			하나의 메모리 공간을 여러 타입으로 겹쳐서 해석할 수 있게 하는 자료형이다.

			struct 와 문법은 비슷하지만 의미는 완전히 다르다.

			struct:
				멤버마다 각자 자기 메모리 공간을 가진다.

			union:
				모든 멤버가 같은 메모리 공간을 공유한다.

			즉 union 에서는
			한 시점에 하나의 값만 "의미 있게" 저장한다고 보는 것이 맞다.

			union 크기:
				멤버들 중 가장 큰 타입의 크기와 같거나,
				정렬 때문에 약간 더 클 수 있다.
		*/

		{
			union mytypes_t
			{
				char c;
				int i;
				float f;
			};

			mytypes_t mytypes;

			mytypes.i = 65;
			std::cout << "mytypes.i = " << mytypes.i << std::endl;

			mytypes.c = 'A';
			std::cout << "mytypes.c = " << mytypes.c << std::endl;
			std::cout << "mytypes.i = " << mytypes.i << std::endl;
			std::cout << std::endl;

			/*
				설명:
				c, i, f 는 서로 다른 메모리를 갖는 것이 아니라
				같은 메모리를 공유한다.

				따라서 한 멤버 값을 바꾸면
				다른 멤버를 읽을 때도 그 영향을 받는다.
			*/
		}

		{
			union mix_t
			{
				int l;
				struct
				{
					short hi;
					short lo;
				} s;
				char c[4];
			};

			mix_t mix;
			mix.l = 0x12345678;

			std::cout << "mix.l    = 0x" << std::hex << mix.l << std::dec << std::endl;
			std::cout << "mix.s.hi = " << mix.s.hi << std::endl;
			std::cout << "mix.s.lo = " << mix.s.lo << std::endl;
			std::cout << "mix.c[0] = " << (int)(unsigned char)mix.c[0] << std::endl;
			std::cout << "mix.c[1] = " << (int)(unsigned char)mix.c[1] << std::endl;
			std::cout << "mix.c[2] = " << (int)(unsigned char)mix.c[2] << std::endl;
			std::cout << "mix.c[3] = " << (int)(unsigned char)mix.c[3] << std::endl;
			std::cout << std::endl;

			/*
				설명:
				같은 4바이트 메모리를
				- int 전체로 볼 수도 있고
				- short 2개로 볼 수도 있고
				- char 4개 배열로 볼 수도 있다.

				이것이 union 의 핵심 아이디어이다.

				주의:
				바이트 순서(endianness), 정렬, 구현 방식에 따라
				결과 해석이 시스템마다 다를 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] union 크기" << std::endl;
			std::cout << "============================================" << std::endl;

			union test_t
			{
				char c;
				int i;
				double d;
			};

			std::cout << "sizeof(test_t) = " << sizeof(test_t) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				union 크기는 보통 가장 큰 멤버 크기에 맞춰진다.
				여기서는 보통 double 이 가장 크다.
			*/
		}

		system("pause");
	}

	void anonymous_unions()
	{
		/*
			📚 익명 공용체 (Anonymous unions)

			union 이 클래스나 구조체의 멤버일 때
			이름 없이 선언할 수 있다.

			이 경우 union 멤버들을
			바깥 객체에서 바로 접근할 수 있다.

			즉:

				struct book2_t
				{
					union
					{
						float dollars;
						int yen;
					};
				};

			이렇게 하면
				book2.dollars
				book2.yen
			처럼 바로 접근 가능하다.
		*/

		{
			struct book1_t
			{
				char title[50];
				char author[50];
				union
				{
					float dollars;
					int yen;
				} price;
			};

			struct book2_t
			{
				char title[50];
				char author[50];
				union
				{
					float dollars;
					int yen;
				};
			};

			book1_t book1 = {};
			book2_t book2 = {};

			book1.price.dollars = 19.99f;
			book2.dollars = 29.99f;

			std::cout << "book1.price.dollars = " << book1.price.dollars << std::endl;
			std::cout << "book2.dollars       = " << book2.dollars << std::endl;
			std::cout << std::endl;

			/*
				설명:
				book1 은 이름 있는 union(price) 이므로
					book1.price.dollars
				처럼 접근해야 한다.

				book2 는 익명 union 이므로
					book2.dollars
				처럼 바로 접근 가능하다.
			*/
		}

		{
			struct PriceInfo
			{
				union
				{
					float dollars;
					int yen;
				};
			};

			PriceInfo p = {};
			p.yen = 1000;

			std::cout << "p.yen = " << p.yen << std::endl;
			std::cout << "같은 메모리를 dollars 와 yen 이 공유한다." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				익명 union 이라고 해도
				여전히 공용 메모리를 사용한다는 점은 같다.

				즉 dollars 와 yen 을 동시에 독립적으로 저장하는 것이 아니다.
			*/
		}

		system("pause");
	}


	void enumerated_types()
	{
		/*
			📚 열거형 (Enumerated types)
			(enum)

			enum 은
			미리 정해 놓은 값들 중 하나를 선택해서 사용하는 타입이다.

			예:
				enum colors_t
				{
					black,
					blue,
					green
				};

			이렇게 하면 colors_t 타입 변수는
			black, blue, green 같은 값들을 가질 수 있다.

			중요:
			기존의 enum 은 내부적으로 int 와 연결되어 있으며,
			암시적으로 int 와 변환될 수 있다.
			이 점 때문에 타입 안전성이 약한 편이다.
		*/

		{
			enum colors_t
			{
				black,
				blue,
				green,
				cyan,
				red,
				purple,
				yellow,
				white
			};

			colors_t mycolor;

			mycolor = blue;

			if (mycolor == green)
				mycolor = red;

			std::cout << "mycolor = " << mycolor << std::endl;
			std::cout << "blue 의 내부 정수값은 보통 1이다." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				기본 enum 은 첫 값이 0부터 시작한다.
				즉 일반적으로:
					black = 0
					blue  = 1
					green = 2
					...
			*/
		}

		{
			enum months_t
			{
				january = 1,
				february,
				march,
				april,
				may,
				june,
				july,
				august,
				september,
				october,
				november,
				december
			};

			months_t m = march;

			std::cout << "march = " << m << std::endl;
			std::cout << "january 는 1부터 시작하도록 직접 지정했다." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				january = 1 로 지정했으므로
				뒤의 값들은 자동으로 2, 3, 4 ... 가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] enum 은 int 와 암시 변환 가능" << std::endl;
			std::cout << "============================================" << std::endl;

			enum number_t
			{
				one = 1,
				two = 2,
				three = 3
			};

			number_t n = two;
			int value = n;

			std::cout << "n     = " << n << std::endl;
			std::cout << "value = " << value << std::endl;
			std::cout << std::endl;

			/*
				설명:
				기존 enum 은 int 와 쉽게 섞일 수 있어서 편하기도 하지만,
				반대로 타입 안정성이 약하다.
			*/
		}

		system("pause");
	}


	void enumerated_types_with_enum_class()
	{
		/*
			📚 enum class
			(범위 있는 열거형, strongly typed enum)

			C++에서는 기존 enum 의 약점을 보완한
			enum class 를 제공한다.

			특징:
			1) int 로 암시 변환되지 않는다
			2) 열거자 이름이 자신의 타입 범위 안에 있다
			3) 타입 안전성이 더 높다

			예:
				enum class Colors
				{
					black,
					blue,
					green
				};

				Colors c = Colors::blue;
		*/

		{
			enum class Colors
			{
				black,
				blue,
				green,
				cyan,
				red,
				purple,
				yellow,
				white
			};

			Colors mycolor = Colors::blue;

			if (mycolor == Colors::blue)
			{
				std::cout << "mycolor is blue" << std::endl;
			}

			std::cout << std::endl;

			/*
				설명:
				enum class 는
					Colors::blue
				처럼 타입 이름을 붙여서 사용해야 한다.

				즉 열거자 이름이 전역으로 그냥 흘러나오지 않는다.
			*/
		}

		{
			enum class EyeColor : char
			{
				blue,
				green,
				brown
			};

			EyeColor eye = EyeColor::green;

			std::cout << "sizeof(EyeColor) = " << sizeof(EyeColor) << std::endl;
			std::cout << "underlying type is char" << std::endl;
			std::cout << std::endl;

			/*
				설명:
				enum class 는 내부 저장 타입(underlying type)을 지정할 수 있다.

				여기서는 char 로 지정했으므로
				보통 크기가 1바이트가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] enum class 는 int 로 자동 변환되지 않음" << std::endl;
			std::cout << "============================================" << std::endl;

			enum class Number
			{
				one = 1,
				two = 2
			};

			Number n = Number::two;

			// int value = n; // 오류
			int value = static_cast<int>(n);

			std::cout << "value = " << value << std::endl;
			std::cout << std::endl;

			/*
				설명:
				enum class 는 타입 안전성을 위해
				int 로 자동 변환되지 않는다.

				필요하면 static_cast<int>(n) 처럼 명시적으로 변환해야 한다.
			*/
		}

		system("pause");
	}

	void Test()
	{
		//type_aliases();

		//unions();

		//anonymous_unions();

		//enumerated_types();

		//enumerated_types_with_enum_class();
	}

}// end of OtherDataTypes