#include "stdafx.h"


namespace Constants
{
	void Literals_what()
	{
		/*
			📚 Literal

			  - 리터럴은 소스 코드에서 '값' 자체를 직접 표기하는 상수입니다.
			  - 변수에 값을 대입하거나, 메시지를 출력하거나, 조건문 등에서 바로 사용할 수 있습니다.
			  - 리터럴의 종류는 다음과 같습니다:

			  1. 정수 리터럴 (Integer Literal)
			    - 10진수, 8진수(0으로 시작), 16진수(0x로 시작) 표기법을 지원합니다.
			    - 예시: 75, 0113, 0x4b
			    - 접미사(suffix)로 타입을 명확히 할 수 있습니다.
			      + u/U: unsigned
				  + l/L: long
				  + ll/LL: long long
			    - 예시: 75u, 75l, 75ul, 75ll

			  2. 부동소수점 리터럴 (Floating-point Literal)
			    - 실수값이나 지수 표기(e)를 사용할 수 있습니다.
			    - 예시: 3.14, 1.2e3, 6.02e23f, 3.14159L
			    - 접미사(suffix)로 타입을 지정할 수 있습니다.
				  + f/F: float
				  + l/L: long double

			  3. 문자 리터럴 (Character Literal)
				- 작은따옴표('')로 감쌉니다.
				- 이스케이프 문자(\n, \t 등)를 사용할 수 있습니다.
				- 예시: 'A', '\n', '\t'

			  4. 문자열 리터럴 (String Literal)
				- 큰따옴표("")로 감쌉니다.
				- 여러 문자열 리터럴을 공백 등으로 나란히 쓰면 하나로 결합됩니다.
				- 다양한 인코딩 접두어(prefix)(u8, u, U, L, R) 지원합니다.
				- 예시: "Hello", "Line1\nLine2", R"(raw string 예시)"

			  5. 불리언 리터럴 (Boolean Literal)
				- true, false
		*/
		{
			// 1. 정수 리터럴
			int iVal = 75;                  // 10진수
			int iOct = 0113;                // 8진수 (75)
			int iHex = 0x4b;                // 16진수 (75)
			unsigned int uiVal = 75u;       // unsigned int
			long lVal = 75l;                // long
			unsigned long ulVal = 75ul;     // unsigned long
			long long llVal = 123456789012345LL; // long long

			// 2. 부동소수점 리터럴
			double dVal = 3.14159;          // double
			float fVal = 6.02e23f;          // float
			long double ldVal = 3.14159L;   // long double

			// 3. 문자/이스케이프 리터럴
			char ch1 = 'A';
			char ch2 = '\n';                // 개행 문자
			char ch3 = '\t';                // 탭 문자

			// 4. 문자열 리터럴
			const char* str1 = "Hello, world!\n";
			const char* str2 = "C++ " "리터럴" " 테스트"; // 여러 문자열 결합

			// 5. 불리언 리터럴
			bool foo = true;
			bool bar = false;

			std::cout << "[정수 리터럴] " << iVal << ", " << iOct << ", " << iHex << std::endl;
			std::cout << "[unsigned/long] " << uiVal << ", " << lVal << ", " << ulVal << ", " << llVal << std::endl;
			std::cout << "[부동소수점] " << dVal << ", " << fVal << ", " << ldVal << std::endl;
			std::cout << "[문자/이스케이프] " << ch1 << " <개행>" << ch2 << " <탭>" << ch3 << "<끝>" << std::endl;
			std::cout << "[문자열] " << str1;
			std::cout << str2 << std::endl;
			std::cout << "[불리언] " << std::boolalpha << foo << ", " << bar << std::endl;

			system("pause");
		}
	}

	void typed_constant_expressions()
	{
		/*
			📚 타입이 있는 상수 표현식 (Typed constant expressions)

			프로그램을 작성하다 보면
			고정된 값(constant value)에 이름을 붙여서 사용하면 편리한 경우가 많다.

			예:
				const double pi = 3.1415926;
				const char tab = '\t';

			이렇게 하면 숫자나 문자 리터럴을 직접 반복해서 쓰는 대신,
			의미 있는 이름으로 사용할 수 있다.

			const 의 의미:
			- 선언과 동시에 값을 정한다.
			- 그 이후에는 값을 바꿀 수 없다.
			- 자료형이 명확하다. (int, double, char 등)

			즉 const 는
			"값이 변하지 않는 타입이 있는 변수"처럼 생각하면 이해하기 쉽다.
		*/

		{
			const double pi = 3.14159;
			const char newline = '\n';

			double r = 5.0;   // 반지름(radius)
			double circle;

			circle = 2 * pi * r;

			std::cout << circle;
			std::cout << newline;

			/*
				출력 결과:
					31.4159

				설명:
				원둘레 공식:
					2 * pi * r

				r = 5.0 이므로
					2 * 3.14159 * 5.0 = 31.4159
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] const 상수는 의미를 분명하게 해준다" << std::endl;
			std::cout << "============================================" << std::endl;

			const int maxPlayers = 100;
			int currentPlayers = 57;

			std::cout << "maxPlayers = " << maxPlayers << std::endl;
			std::cout << "currentPlayers = " << currentPlayers << std::endl;
			std::cout << std::endl;

			/*
				설명:
				100 이라는 숫자를 그냥 쓰는 것보다
				maxPlayers 라는 이름을 쓰면
				그 값의 의미를 더 쉽게 이해할 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] const 값은 변경할 수 없다" << std::endl;
			std::cout << "============================================" << std::endl;

			const int year = 2026;

			std::cout << "year = " << year << std::endl;
			std::cout << "const 로 선언한 값은 이후에 바꿀 수 없다." << std::endl;
			std::cout << std::endl;

			/*
				예를 들어 아래 코드는 오류가 된다:

					year = 2030;   // 컴파일 오류

				설명:
				const 상수는 읽기 전용(read-only)이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 문자 상수 사용" << std::endl;
			std::cout << "============================================" << std::endl;

			const char tab = '\t';
			const char newline = '\n';

			std::cout << "Name" << tab << "Score" << newline;
			std::cout << "Justin" << tab << 100 << newline;
			std::cout << std::endl;

			/*
				설명:
				tab 은 탭 문자,
				newline 은 줄바꿈 문자이다.

				이처럼 문자 상수에도 의미 있는 이름을 붙여서 사용할 수 있다.
			*/
		}

		system("pause");
	}

	void preprocessor_definitions()
	{
		/*
			📚 전처리기 정의 (#define)

			상수에 이름을 붙이는 또 다른 방법은
			전처리기 정의(preprocessor definition)를 사용하는 것이다.

			형태:
				#define 이름 치환값

			예:
				#define PI 3.14159
				#define NEWLINE '\n'

			의미:
			컴파일이 시작되기 전에,
			소스 코드 안의 이름(identifier)을
			지정된 치환값(replacement)으로 단순 치환한다.

			중요:
			이 치환은 전처리기(preprocessor)가 수행하며,
			컴파일러가 타입이나 문법을 검사하기 전에 먼저 이루어진다.

			즉,
			#define 은 "타입이 있는 상수"가 아니라
			"문자 그대로 바꿔치기하는 규칙"에 가깝다.

			그래서 const 보다 더 단순하고 강력하지만,
			그만큼 실수도 생기기 쉽다.
		*/

		#define PI 3.14159
		#define NEWLINE '\n'

		{
			double r = 5.0;   // 반지름(radius)
			double circle;

			circle = 2 * PI * r;

			std::cout << circle;
			std::cout << NEWLINE;

			/*
				출력 결과:
					31.4159

				설명:
				전처리 단계에서
					PI      -> 3.14159
					NEWLINE -> '\n'
				로 바뀐 후 컴파일된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] #define 도 이름 있는 상수처럼 보일 수 있다" << std::endl;
			std::cout << "============================================" << std::endl;

			#define MAX_USERS 500

			int currentUsers = 123;

			std::cout << "MAX_USERS = " << MAX_USERS << std::endl;
			std::cout << "currentUsers = " << currentUsers << std::endl;
			std::cout << std::endl;

			/*
				설명:
				코드 안의 MAX_USERS 는
				전처리 단계에서 500 으로 치환된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] #define 은 타입이 없다" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "PI = " << PI << std::endl;
			std::cout << "PI * 2 = " << PI * 2 << std::endl;
			std::cout << std::endl;

			/*
				설명:
				#define PI 3.14159
				는 const double PI = 3.14159; 와 다르다.

				#define 은 단순히 글자를 바꾸는 것이지,
				실제로 double 타입 변수를 만드는 것이 아니다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] #define 사용 시 주의점" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "#define 은 타입 검사 없이 단순 치환이므로" << std::endl;
			std::cout << "예상하지 못한 문제를 만들 수 있다." << std::endl;
			std::cout << "현대 C++에서는 상수값에는 보통 const 또는 constexpr 를 더 선호한다." << std::endl;
			std::cout << std::endl;

			/*
				예를 들어:
					#define VALUE 10 + 20

				이런 식으로 괄호 없이 정의하면
				사용 위치에 따라 의도와 다른 결과가 생길 수 있다.

				그래서 단순 상수값은
				const 가 더 안전하고 읽기 좋다.
			*/
		}

		system("pause");
	}

	void Test()
	{
		//Literals_what();

		//typed_constant_expressions();

		//preprocessor_definitions();
	}
}//end of Constants