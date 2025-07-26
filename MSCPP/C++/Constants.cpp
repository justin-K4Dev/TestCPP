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
			Typed constant expressions

			Sometimes, it is just convenient to give a name to a constant value :

			const double pi = 3.1415926;
			const char tab = '\t';

			We can then use these names instead of the literals they were defined to:
		*/
		{
			const double pi = 3.14159;
			const char newline = '\n';

			double r = 5.0;	// radius
			double circle;

			circle = 2 * pi * r;
			std::cout << circle;
			std::cout << newline;

			system("pause");

			/*
			output:
				31.4159
			*/
		}
	}

	void preprocessor_definitions()
	{
		/*
			Preprocessor definitions (#define)

			Another mechanism to name constant values is the use of preprocessor definitions.
			They have the following form:

				#define identifier replacement

			After this directive, any occurrence of identifier in the code is interpreted as replacement,
			where replacement is any sequence of characters (until the end of the line).
			This replacement is performed by the preprocessor, and happens before the program is compiled,
			thus causing a sort of blind replacement: the validity of the types
			or syntax involved is not checked in any way.

			For example:
		*/
		{
			#define PI 3.14159
			#define NEWLINE '\n'

			double r = 5.0;               // radius
			double circle;

			circle = 2 * PI * r;
			std::cout << circle;
			std::cout << NEWLINE;

			system("pause");

			/*
			output:
				31.4159
			*/
		}
	}

	void Test()
	{
		//Literals_what();

		//typed_constant_expressions();

		//preprocessor_definitions();
	}
}// end of Constants