#if defined(_MSC_VER)
	#pragma fenv_access(on)   // MSVC 전용(가능하면 /fp:strict도 같이)
#endif
#pragma STDC FENV_ACCESS ON  // 컴파일러가 FP 환경을 고려하도록 힌트(반드시 보장되진 않음)

#include "stdafx.h"

#include <cfenv>   // fenv.h (C++11), MSVC에서 일부 제약이 있을 수 있음


namespace VariablesAndTypes
{
	void variables_n_types()
	{
		/*
			📚 변수와 타입(자료형) 개요

			프로그래밍이 처리하는 핵심은,
			값(데이터)을 저장해두고, 그 값을 다시 꺼내 계산하고, 조건을 판단하고,
			결과를 다른 곳으로 전달하는 과정입니다.
			이때 “값을 저장하는 상자”가 변수(variable)이고,
			“그 상자에 어떤 종류의 값이 들어가는지에 대한 규칙”이 타입(type, 자료형)입니다.

			---------------------------------------------------------------------------------------
			1) 변수(variable)란?
			---------------------------------------------------------------------------------------
			- 변수는 메모리의 어떤 공간을 대표하는 이름입니다.
			- 그 공간에 값을 “저장(대입)”할 수 있고, 나중에 “읽어서” 계산에 사용할 수 있습니다.
			- 즉, 변수는 값 자체가 아니라, 값을 담는 ‘주소가 있는 자리’를 사람이 다루기 쉽게 이름 붙인 것입니다.

			예)
				a = 5;        // a라는 변수 공간에 5 저장
				b = 2;        // b라는 변수 공간에 2 저장
				a = a + 1;    // a에 들어있던 값을 읽어서 1 더한 뒤 다시 a에 저장
				result = a - b; // a와 b를 읽어서 빼고 result에 저장

			이런 흐름은 “기억해둔 숫자를 다시 꺼내 바꾸는 과정”과 동일합니다.

			---------------------------------------------------------------------------------------
			2) 왜 타입(type, 자료형)이 필요한가?
			---------------------------------------------------------------------------------------
			컴퓨터 메모리는 결국 ‘0과 1’의 나열이지만,
			그 비트들을 어떤 의미로 해석할지는 규칙이 필요합니다.

			- 01000001 이라는 비트가
			  * 정수 65일 수도 있고
			  * 문자 'A'일 수도 있고
			  * 어떤 구조체의 일부일 수도 있습니다.

			C++에서 타입은 다음을 결정합니다.
			- 메모리 크기(몇 바이트를 쓰는가)
			- 값의 해석 방식(정수? 실수? 문자? 주소?)
			- 가능한 연산(더하기/비교/비트연산/포인터연산 등)
			- 오버플로우/정밀도/부호 같은 표현 범위 특성

			---------------------------------------------------------------------------------------
			3) C++의 대표적인 기본 타입들(감 잡기)
			---------------------------------------------------------------------------------------
			(1) 정수형
			- int          : 가장 흔한 “정수”
			- short, long, long long : 더 작은/더 큰 범위의 정수
			- unsigned int : 음수가 없는 정수(0 이상만)

			(2) 실수형
			- float  : 단정도(메모리 적게, 정밀도 상대적으로 낮음)
			- double : 배정도(기본 실수처럼 가장 많이 사용)
			- long double : 더 높은 정밀도(플랫폼 의존)

			(3) 문자/문자열 관련
			- char          : 1바이트 문자/바이트 값
			- wchar_t, char16_t, char32_t : 유니코드 문자 표현용 타입들
			- 문자열은 보통 std::string / std::wstring 같은 라이브러리 타입으로 다룹니다.

			(4) 논리형
			- bool : true / false

			(5) “주소”를 다루는 포인터
			- int* p;  // int가 저장된 메모리 주소를 담는 변수
			포인터는 “값”이 아니라 “어딘가를 가리키는 주소”를 저장합니다.

			---------------------------------------------------------------------------------------
			4) 변수 선언(define)과 초기화(initialization)
			---------------------------------------------------------------------------------------
			C++에서는 보통 “타입 + 이름”으로 변수를 선언합니다.

			예)
				int a;        // int 타입 변수 a 선언 (초기값은 정해져 있지 않을 수 있음)
				int b = 2;    // 선언과 동시에 2로 초기화
				double pi = 3.14159;
				bool ok = true;

			중요한 포인트:
			- “선언”은 변수를 만들고 타입을 정하는 행위
			- “초기화”는 처음 값을 넣는 행위
			- “대입(assignment)”은 이미 존재하는 변수에 값을 다시 넣는 행위

			---------------------------------------------------------------------------------------
			5) 식별자(identifier) 이름 규칙/관례
			---------------------------------------------------------------------------------------
			- 영문자/숫자/언더스코어(_) 사용 가능
			- 숫자로 시작 불가
			- C++ 예약어(int, class, return 등)는 사용 불가
			- 보통 의미 있는 이름을 권장:
			  * count, totalScore, playerId, resultValue 등

			---------------------------------------------------------------------------------------
			6) 타입이 다르면 연산 결과도 달라질 수 있다
			---------------------------------------------------------------------------------------
			같은 “나눗셈”이라도 타입에 따라 결과가 달라집니다.

			예)
				int a = 5, b = 2;
				int x = a / b;        // 정수 나눗셈 -> 2 (소수점 버림)
				double y = a / b;     // 여전히 2 (a/b가 먼저 int로 계산됨)
				double z = (double)a / b; // 2.5

			즉, “어떤 타입으로 계산하느냐”는 결과에 직접 영향을 줍니다.

			---------------------------------------------------------------------------------------
			7) 한 줄 결론
			---------------------------------------------------------------------------------------
			- 변수는 “값을 저장하는 메모리 공간의 이름”
			- 타입은 “그 공간에 들어있는 비트를 어떤 값으로 해석하고 어떻게 다룰지 정하는 규칙”
			이 두 가지가 결합되어 C++ 프로그램이 데이터를 저장하고, 계산하고, 제어 흐름을 만들 수 있습니다.
		*/

		//-----------------------------------------------------------------------------------------
		// 1) 변수: 저장/읽기/대입(갱신) 테스트
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[1] Variables: store/read/assign\n";

			int a = 5;            // 초기화
			int b = 2;
			int result = 0;

			std::cout << "init: a=" << a << ", b=" << b << ", result=" << result << "\n";

			a = a + 1;            // 대입(갱신)
			result = a - b;

			std::cout << "after: a=" << a << " (a=a+1), result=" << result << " (a-b)\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 2) 타입이 필요한 이유(같은 비트, 다른 해석) 테스트
		//   - union을 이용해 같은 메모리(비트)를 int/float로 해석
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[2] Type meaning: same bits, different interpretation\n";

			union Bits
			{
				unsigned int u;
				float f;
			};

			Bits x;
			x.u = 0x3F800000u; // IEEE-754 float에서 1.0f의 비트 패턴으로 널리 알려진 값

			std::cout << "bits (hex) = 0x" << std::hex << x.u << std::dec << "\n";
			std::cout << "as float   = " << x.f << "\n";

			// 반대로 float 넣고 정수 비트로 보기
			Bits y;
			y.f = 2.5f;
			std::cout << "float 2.5f as bits (hex) = 0x" << std::hex << y.u << std::dec << "\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 3) 기본 타입들 테스트 (크기/범위, 출력)
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[3] Built-in types: size and range\n";

			std::cout << "sizeof(bool)        = " << sizeof(bool) << "\n";
			std::cout << "sizeof(char)        = " << sizeof(char) << "\n";
			std::cout << "sizeof(int)         = " << sizeof(int) << "\n";
			std::cout << "sizeof(long)        = " << sizeof(long) << "\n";
			std::cout << "sizeof(long long)   = " << sizeof(long long) << "\n";
			std::cout << "sizeof(float)       = " << sizeof(float) << "\n";
			std::cout << "sizeof(double)      = " << sizeof(double) << "\n";

			std::cout << "int min/max         = "
				<< std::numeric_limits<int>::min() << " / "
				<< std::numeric_limits<int>::max() << "\n";

			std::cout << "unsigned int max    = "
				<< std::numeric_limits<unsigned int>::max() << "\n";

			std::cout << "float min/max       = "
				<< std::numeric_limits<float>::lowest() << " / "
				<< std::numeric_limits<float>::max() << "\n";

			std::cout << "double min/max      = "
				<< std::numeric_limits<double>::lowest() << " / "
				<< std::numeric_limits<double>::max() << "\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 4) 선언 vs 초기화 vs 대입 테스트
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[4] Declaration vs Initialization vs Assignment\n";

			int a;        // 선언 (주의: 초기값이 정해져있지 않을 수 있음)
			// cout << a << "\n"; // 안전하지 않으니 출력하지 않는 것을 권장

			int b = 10;   // 초기화
			std::cout << "b initialized = " << b << "\n";

			b = 20;       // 대입
			std::cout << "b assigned    = " << b << "\n";

			int c{ 30 };    // 중괄호 초기화(Uniform init)
			std::cout << "c initialized with {} = " << c << "\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 5) 식별자 규칙 테스트(컴파일되는 예)
		//   - “컴파일 오류 예”는 주석으로 남겨둠
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[5] Identifier rules (compile-safe examples)\n";

			int _value = 1;
			int value2 = 2;
			int player_id = 3;

			std::cout << "_value=" << _value << ", value2=" << value2 << ", player_id=" << player_id << "\n";

			// 아래는 컴파일 에러 예시(주석 해제하면 오류)
			// int 2value = 10;     // 숫자로 시작 불가
			// int class = 3;       // 예약어 사용 불가
			// int value-1 = 0;     // '-' 사용 불가

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 6) 타입에 따라 결과가 달라지는 연산 테스트 (정수/실수 나눗셈)
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[6] Operation depends on type: integer vs floating division\n";

			int a = 5, b = 2;

			int x = a / b;               // 정수 나눗셈
			double y = a / b;            // (a/b)가 먼저 int로 계산됨
			double z = (double)a / b;    // 실수 나눗셈

			std::cout << "int x = a/b              = " << x << "\n";
			std::cout << "double y = a/b           = " << y << "\n";
			std::cout << "double z = (double)a/b   = " << z << "\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 7) 오버플로우(정수) / 정밀도(실수) 테스트
		//   - signed 오버플로우는 C++에서 정의되지 않은 동작(UB)이 될 수 있어
		//     여기서는 unsigned로 “랩어라운드” 관찰
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[7] Overflow (unsigned wrap) & Precision\n";

			unsigned int u = std::numeric_limits<unsigned int>::max();
			std::cout << "unsigned max = " << u << "\n";
			u = u + 1; // 랩어라운드(모듈러 연산처럼 동작)
			std::cout << "unsigned max + 1 = " << u << " (wrap-around)\n";

			// 실수 정밀도
			float f = 0.1f;
			double d = 0.1;

			std::cout << "float  0.1f * 10 = " << (f * 10) << "\n";
			std::cout << "double 0.1  * 10 = " << (d * 10) << "\n";

			// 0.1을 10번 더했을 때 오차
			float fs = 0.0f;
			double ds = 0.0;
			for (int i = 0; i < 10; ++i) { fs += 0.1f; ds += 0.1; }

			std::cout << "float  sum 0.1f x10 = " << fs << "\n";
			std::cout << "double sum 0.1  x10 = " << ds << "\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 8) 포인터(주소) 타입 테스트
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[8] Pointer: address vs value\n";

			int value = 42;
			int* p = &value; // value의 주소를 저장

			std::cout << "value      = " << value << "\n";
			std::cout << "&value     = " << &value << "\n";
			std::cout << "p (address)= " << p << "\n";
			std::cout << "*p (value) = " << *p << "\n";

			*p = 100; // 포인터로 원본 값 변경
			std::cout << "after *p=100, value = " << value << "\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 9) 문자열 타입 테스트(std::string)
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[9] std::string basics\n";

			std::string s = "Hello";
			s += ", C++";

			std::cout << "s = " << s << "\n";
			std::cout << "length = " << s.size() << "\n";
			std::cout << "s[0] = " << s[0] << "\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 10) 간단한 타입 추론(auto) 테스트
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[10] auto type deduction\n";

			auto a = 10;       // int로 추론
			auto b = 3.14;     // double로 추론
			auto c = 3.14f;    // float로 추론
			auto d = true;     // bool로 추론
			auto e = "ABC";    // const char* 로 추론(문자열 리터럴)

			std::cout << "a=" << a << " (type: " << typeid(a).name() << ")\n";
			std::cout << "b=" << b << " (type: " << typeid(b).name() << ")\n";
			std::cout << "c=" << c << " (type: " << typeid(c).name() << ")\n";
			std::cout << "d=" << d << " (type: " << typeid(d).name() << ")\n";
			std::cout << "e=" << e << " (type: " << typeid(e).name() << ")\n";

			// typeid().name() 출력은 컴파일러마다 문자열이 다를 수 있음(테스트 목적)

			system("pause");
		}
	}

	//---------------------------------------------------------------------------------------------

	void identifiers()
	{
		/*
			📚 식별자(Identifiers)

			C++에서 "식별자(identifier)"는 변수/함수/클래스/네임스페이스 등
			프로그래머가 이름을 붙여야 하는 대상에 사용하는 “이름”입니다.

			---------------------------------------------------------------------------------------
			1) 식별자에 사용할 수 있는 문자
			---------------------------------------------------------------------------------------
			일반적으로 식별자는 다음 문자의 조합으로 구성됩니다.

			- 영문자(A~Z, a~z)
			- 숫자(0~9)
			- 밑줄(언더스코어) '_'

			하지만 다음은 식별자에 포함될 수 없습니다.
			- 공백(space)
			- 구두점/특수기호(예: '-', '+', '!', '@', '#', '.', ',' 등)
			- 기타 기호 문자들

			---------------------------------------------------------------------------------------
			2) 시작 규칙(첫 글자 규칙)
			---------------------------------------------------------------------------------------
			- 식별자는 숫자로 시작할 수 없습니다.
			  예) 2value  (X)

			- 일반적으로는 영문자 또는 '_' 로 시작할 수 있습니다.
			  예) value, _value

			---------------------------------------------------------------------------------------
			3) '_' 로 시작하는 이름에 대한 주의(관례 + 표준 예약 영역)
			---------------------------------------------------------------------------------------
			C++ 표준 라이브러리/컴파일러는 내부적으로 특정 패턴의 이름을 “예약”해 두는 경우가 있습니다.
			특히 다음은 피하는 것이 좋습니다.

			- '_' 로 시작하면서, 두 번째 문자가 대문자인 경우: _Xxx
			- '__' (연속된 밑줄 두 개)가 포함되는 경우: __something

			이런 이름들은 구현(컴파일러/표준 라이브러리)이 사용할 가능성이 있어
			충돌/예상치 못한 문제를 유발할 수 있습니다.

			결론:
			- 사용자 코드에서는 _ 로 시작하는 이름 자체를 가급적 피하고,
			  필요하면 suffix(뒤에 붙이는 방식)나 명확한 접두어(prefix)를 쓰는 편이 안전합니다.
			  예) my_value, player_id, cfgPath 등

			---------------------------------------------------------------------------------------
			4) 예약어(키워드)는 식별자로 사용할 수 없다
			---------------------------------------------------------------------------------------
			int, class, return 같은 C++ 키워드(keyword)는 언어 문법에서 이미 의미가 정해져 있습니다.
			따라서 프로그래머가 만든 식별자는 키워드와 동일한 이름을 사용할 수 없습니다.

			예) int int = 3;   (X)  // int는 키워드

			(참고) 컴파일러/표준에 따라 추가 예약어/확장 키워드가 있을 수도 있습니다.

			---------------------------------------------------------------------------------------
			5) 대소문자 구분(case-sensitive)
			---------------------------------------------------------------------------------------
			C++는 대소문자를 구분하는 언어입니다.
			따라서 다음은 모두 다른 이름입니다.

				result
				Result
				RESULT

			즉, 철자가 같아 보여도 대소문자가 다르면 완전히 다른 식별자입니다.

			---------------------------------------------------------------------------------------
			6) 실무에서의 추천 네이밍 규칙(가독성/유지보수 관점)
			---------------------------------------------------------------------------------------
			- 의미가 드러나는 이름을 사용하기:
			  a, b 같은 이름은 작은 예제에는 좋지만, 실제 코드에서는 맥락이 사라집니다.
			  예) totalCount, maxHp, playerId

			- 관례를 팀/프로젝트 기준으로 통일하기:
			  * camelCase  : playerId, totalCount
			  * PascalCase : PlayerId, TotalCount (클래스/함수에 많이 사용)
			  * snake_case : player_id, total_count

			- 약어 남발을 피하기:
			  dev가 아닌 사람이 봐도 이해되는 정도의 약어만 사용하기.

			---------------------------------------------------------------------------------------
			7) “가능”과 “권장”은 다르다
			---------------------------------------------------------------------------------------
			문법적으로 가능한 이름과, 유지보수에 좋은 이름은 다를 수 있습니다.
			컴파일은 되지만 읽기 어려운 이름(예: l1I0O, __tmp__)은 피하는 것이 좋습니다.
		*/

		//-----------------------------------------------------------------------------------------
		// 1) 컴파일되는 것/안 되는 것을 분리해서 확인하기
		//-----------------------------------------------------------------------------------------
		{
			// 컴파일 되는 것들...
			{
				std::cout << "\n[Test] Identifiers that compile OK\n";

				int value = 10;
				int value2 = 20;
				int player_id = 30;
				int totalCount = 40;
				int Result = 50;       // result와 다름(대소문자 구분)
				int result = 60;

				std::cout << "value=" << value << "\n";
				std::cout << "value2=" << value2 << "\n";
				std::cout << "player_id=" << player_id << "\n";
				std::cout << "totalCount=" << totalCount << "\n";
				std::cout << "Result=" << Result << "\n";
				std::cout << "result=" << result << "\n";

				// '_' 로 시작하는 식별자도 문법상 가능하긴 함(권장X)
				int _temp = 123;
				std::cout << "_temp=" << _temp << " (valid but usually not recommended)\n";

				system("pause");
			}

			// 컴파일 에러 예제 모음 (주석 해제하면 직접 확인 가능)
			{
				// int 2value = 10;       // (X) 숫자로 시작 불가
				// int value - 1 = 20;    // (X) '-' 사용 불가
				// int my value = 30;     // (X) 공백 사용 불가
				// int class = 40;        // (X) 키워드 사용 불가
				// int return = 50;       // (X) 키워드 사용 불가
				// int __tmp = 60;        // (주의) 구현 예약 가능성(피하는 게 좋음)
				// int _X = 70;           // (주의) _ + 대문자 패턴은 예약 영역 가능성
			}
		}

		//-----------------------------------------------------------------------------------------
		// 2) “대소문자 구분” 테스트: 서로 다른 변수임을 보여줌
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[Test] Case sensitivity\n";

			int score = 1;
			int Score = 2;
			int SCORE = 3;

			std::cout << "score=" << score << "\n";
			std::cout << "Score=" << Score << "\n";
			std::cout << "SCORE=" << SCORE << "\n";

			system("pause");
		}

		//-----------------------------------------------------------------------------------------
		// 3) “가독성” 테스트: 같은 기능이라도 이름이 의미를 얼마나 주는지 비교
		//-----------------------------------------------------------------------------------------
		{
			std::cout << "\n[Test] Readability comparison\n";

			// 나쁜 예(예제에서만)
			int a = 100;
			int b = 20;
			int c = a - b;

			// 좋은 예(같은 의미지만 맥락이 살아남)
			int maxHp = 100;
			int damage = 20;
			int remainingHp = maxHp - damage;

			std::cout << "bad naming: c=" << c << "\n";
			std::cout << "good naming: remainingHp=" << remainingHp << "\n";

			system("pause");
		}
	}

	void printLine(const char* title)
	{
		std::cout << "\n============================================================================\n";
		std::cout << title << "\n";
		std::cout << "============================================================================\n";
	}

	//---------------------------------------------------------------------------------------------

	int checkFPClassAnsiC(double x, __out std::string& s)
	{
		/*
			📚 ANSI-C(표준 C/C++) 방식의 부동소수점 분류

			  - fpclassify(x)는 <cmath>에서 제공되는 매크로/함수로,
				부동소수점 값 x를 다음 5가지 범주 중 하나로 분류합니다.

				FP_NAN        : NaN (Not-a-Number)
				FP_ZERO       : +0 또는 -0
				FP_NORMAL     : 정규화(normalized) 값
				FP_SUBNORMAL  : 비정규화(subnormal/denormal) 값
				FP_INFINITE   : +INF 또는 -INF

			  - 반환값은 위 FP_* 상수 중 하나이며,
				s에는 사람이 보기 쉬운 설명 문자열을 저장합니다.

			  - 장점:
				* 표준 함수라 플랫폼 독립적(이식성 좋음)
				* C/C++ 전반에서 공통적으로 사용할 수 있음

			  - 단점/차이점:
				* Windows/MSVC의 _fpclass처럼 “세부 분류”는 하지 않습니다.
					예) Signaling NaN vs Quiet NaN 구분 X
						+0 vs -0 구분 X
						+INF vs -INF 구분 X
				* 즉, “표준 수준의 큰 분류”만 필요할 때 적합합니다.
		*/

		// (1) fpclassify로 부동소수점 상태 분류
		//     결과는 FP_NAN / FP_ZERO / FP_NORMAL / FP_SUBNORMAL / FP_INFINITE 중 하나
		int i = fpclassify(x);

		// (2) 분류 결과에 따라 설명 문자열 설정
		switch (i)
		{
		case FP_NAN:
			// NaN: 수학적으로 정의되지 않은 결과(0/0, sqrt(-1) 등)에서 발생
			s = "NaN";
			break;

		case FP_ZERO:
			// 0: +0 또는 -0 (표준 분류에서는 부호를 따로 구분하지 않음)
			s = "zero (0)";
			break;

		case FP_NORMAL:
			// 정규화(normalized): 일반적인 대부분의 실수 값
			s = "normalized";
			break;

		case FP_SUBNORMAL:
			// 비정규화(subnormal/denormal): 0에 매우 가까운 아주 작은 값(정밀도 손실 가능)
			s = "subnormal";
			break;

		case FP_INFINITE:
			// 무한대: 오버플로우(너무 큰 값) 또는 1.0/0.0 같은 연산에서 발생 가능
			// 표준 분류에서는 +INF/-INF를 구분하지 않음
			s = "infinity (INF)";
			break;

		default:
			// 이론상 거의 없지만 안전을 위해 처리
			s = "unknown fpclassify result";
			break;
		}

		// (3) 분류 결과 상수 반환
		return i;
	}
	
	int checkFPClassWin32(double x, __out std::string& s)
	{
		/*
			📚 Windows/MSVC 방식의 부동소수점 분류

			  - _fpclass()를 사용해서 double 값이 NaN/INF/정규/비정규/±0 인지 분류합니다.
			  - 반환값은 _FPCLASS_* 상수 중 하나이며, s에는 사람이 읽을 설명 문자열을 넣습니다.
		*/

		// MSVC CRT의 _fpclass는 부동소수점의 상태를 세밀하게 분류해줍니다.
		// (ANSI-C의 fpclassify보다 상세: SNaN/QNaN, +0/-0, +INF/-INF 등)
		int i = _fpclass(x);

		// 분류 결과에 따라 설명 문자열을 저장
		switch (i)
		{
		case _FPCLASS_SNAN: s = "Signaling NaN";                break; // 신호 NaN
		case _FPCLASS_QNAN: s = "Quiet NaN";                    break; // 조용한 NaN
		case _FPCLASS_NINF: s = "Negative infinity (-INF)";     break; // -무한대
		case _FPCLASS_NN:   s = "Negative normalized non-zero"; break; // 음수 정규값(0 아님)
		case _FPCLASS_ND:   s = "Negative denormalized";        break; // 음수 비정규(denormal/subnormal)
		case _FPCLASS_NZ:   s = "Negative zero (-0)";           break; // -0
		case _FPCLASS_PZ:   s = "Positive 0 (+0)";              break; // +0
		case _FPCLASS_PD:   s = "Positive denormalized";        break; // 양수 비정규
		case _FPCLASS_PN:   s = "Positive normalized non-zero"; break; // 양수 정규값(0 아님)
		case _FPCLASS_PINF: s = "Positive infinity (+INF)";     break; // +무한대

		default:
			// (이론상) 여기로 들어오지 않는 것이 일반적이지만, 확장/예외 상황 대비
			s = "Unknown FP class";
			break;
		}
		
		return i;
	}

	template<typename T>
	bool CheckValue(T v, std::string& outString)
	{
		/*
			📚 타입별 출력 문자열 준비 + (float/double) 특수값 필터링

			  - 템플릿 타입 T를 보고 "타입명"과 "printf 포맷"을 결정합니다.
			  - float/double이라면 NaN/INF/denormal 같은 특수 상태는
			    정상 출력 대신 설명 문자열만 반환하고 false를 리턴합니다.
			  - 정상 값이면 "<값> : <타입명> = " 형태의 헤더 문자열을 만들어 outString에 담고 true 리턴.
		*/

		// float/double일 때만 FP 특수값 검사 실행
		bool isCheckFP = false;

		// 사람이 보기 좋은 타입 이름(문자열)
		std::string type;

		// printf 포맷 문자열을 만들 버퍼
		// 예: "%d : %s = "  또는 "%f : %s = "
		char buffer[1024];
		buffer[0] = 0; // 빈 문자열로 초기화(원본은 '0'였는데, 실제로는 0이 안전)

		// ------------------------------------------------------------
		// (1) 타입 판별 + 타입명/포맷 결정
		// ------------------------------------------------------------
		if (typeid(T) == typeid(char)) {
			type = "char";
			strcpy_s(buffer, "%c");
		}
		else if (typeid(T) == typeid(unsigned char)) {
			type = "unsigned char";
			strcpy_s(buffer, "%c"); // 출력 목적이면 %u가 더 적절할 수 있음(원본 의도 유지)
		}
		else if (typeid(T) == typeid(short)) {
			type = "short";
			strcpy_s(buffer, "%d");
		}
		else if (typeid(T) == typeid(unsigned short)) {
			type = "unsigned short";
			strcpy_s(buffer, "%u");
		}
		else if (typeid(T) == typeid(int)) {
			type = "int";
			strcpy_s(buffer, "%d");
		}
		else if (typeid(T) == typeid(unsigned int)) {
			type = "unsigned int";
			strcpy_s(buffer, "%u");
		}
		else if (typeid(T) == typeid(__int64)) {
			type = "__int64";
			strcpy_s(buffer, "%I64d");
		}
		else if (typeid(T) == typeid(unsigned __int64)) {
			type = "unsigned __int64";
			strcpy_s(buffer, "%I64u"); // 원본은 %I64d였지만 unsigned는 u가 정확
		}
		else if (typeid(T) == typeid(float)) {
			type = "float";
			strcpy_s(buffer, "%f");
			isCheckFP = true; // float는 FP 특수값 검사
		}
		else if (typeid(T) == typeid(double)) {
			type = "double";
			strcpy_s(buffer, "%f");
			isCheckFP = true; // double도 FP 특수값 검사
		}
		else {
			// 지원하지 않는 타입이면 실패 처리
			outString = "unknown type !!!";
			return false;
		}

		// ------------------------------------------------------------
		// (2) float/double 특수값 검사
		// ------------------------------------------------------------
		if (isCheckFP)
		{
			std::string strResult;
			int fp_result = checkFPClassWin32((double)v, __out strResult);

			// 특수값을 “정상 숫자 출력”에서 제외
			// - INF / NaN / denormal 등은 출력 목적에 따라 별도 취급하는 것이 안전
			if (   _FPCLASS_PINF == fp_result
				|| _FPCLASS_NINF == fp_result
				|| _FPCLASS_PD == fp_result
				|| _FPCLASS_ND == fp_result
				|| _FPCLASS_QNAN == fp_result
				|| _FPCLASS_SNAN == fp_result )
			{
				outString = strResult; // 설명 문자열만 내보냄
				return false;
			}
		}

		// ------------------------------------------------------------
		// (3) 출력용 헤더 문자열 생성
		// ------------------------------------------------------------
		// buffer는 현재 "%d" 또는 "%f" 같은 포맷만 들어있습니다.
		// 여기 뒤에 " : %s = "를 붙여서,
		//   예) "%d : %s = "  또는 "%f : %s = "
		// 형태로 만든 다음 sprintf로 value와 type을 넣습니다.
		strcat_s(buffer, " : %s = ");

		char message[1024];

		// message 예:
		//   "10 : int = "
		//   "0.100000 : float = "
		sprintf_s(message, buffer, v, type.c_str());

		outString = message;
		return true;
	}

	template<typename T>
	void printBits(T value)
	{
		/*
			📚 값의 “순수 비트열” 출력(4비트마다 공백)

			  - 값이 메모리에 어떤 0/1 패턴으로 저장되는지 그대로 출력합니다.
			  - float/double의 NaN/INF/denormal은 CheckValue에서 걸러서 설명만 출력합니다.
		*/

		std::string outString;

		// (1) 출력 가능한 정상 값인지 확인 + 헤더 문자열 얻기
		if (!CheckValue(value, __out outString)) {
			// 특수값이거나 지원하지 않는 타입이면 설명만 출력하고 종료
			printf("%s\n", outString.c_str());
			return;
		}

		// (2) "값 : 타입 = " 헤더 먼저 출력
		printf("%s", outString.c_str());

		// (3) value의 메모리 내용을 바이트 배열로 해석
		//     unsigned char는 1바이트이므로 raw byte 읽기에 적합
		unsigned char* b = (unsigned char*)&value;

		bool begin = true;

		// (4) MSB(가장 높은 바이트) -> LSB 순으로 출력
		for (int i = (int)sizeof(T) - 1; i >= 0; --i)
		{
			// 각 바이트의 bit7..bit0을 출력
			for (int j = 7; j >= 0; --j)
			{
				// (1<<j)로 해당 비트만 마스킹한 뒤 0/1로 정리
				unsigned char bit = (b[i] >> j) & 1;

				// 보기 좋게 4비트마다 공백(첫 출력은 제외)
				if (!begin && ((j + 1) % 4 == 0)) {
					printf(" ");
				}
				begin = false;

				printf("%u", (unsigned)bit);
			}
		}
		puts("");
	}

	template<typename T>
	void printBitFormatOfType(T value)
	{
		/*
			📚 타입별 “의미 경계”를 구분해서 비트 출력

			  - int 계열 : 최상위 비트(부호 비트 위치) 뒤에 공백, 이후 4비트 그룹
			  - float    : [sign 1][exp 8][mantissa 23] 경계에 공백을 넣어 보기 쉽게 출력
			  - double   : [sign 1][exp 11][mantissa 52] 경계에 공백을 넣어 보기 쉽게 출력

			  * 주의
			    - float/double이 IEEE-754라는 전제 하에 시각화하는 출력입니다.
				- mantissa를 5비트 단위로 끊는 것은 표준 경계가 아니라 “가독성 목적”입니다.
		*/

		std::string outString;

		// (1) 출력 가능 여부 확인 + 헤더 문자열 얻기
		if (true != CheckValue(value, __out outString)) {
			printf("%s\n", outString.c_str());
			return;
		}

		// (2) 헤더 출력
		printf("%s", outString.c_str());

		// (3) 바이트 포인터로 메모리 읽기
		unsigned char* b = (unsigned char*)&value;

		// bitPos는 전체 비트에서 “몇 번째 비트를 출력 중인지” (0부터 증가)
		for (int i = (int)sizeof(T) - 1, bitPos = 0; i >= 0; --i)
		{
			for (int j = 7; j >= 0; --j, ++bitPos)
			{
				unsigned char bit = (b[i] >> j) & 1;

				// ----------------------------
				// 타입별 공백 삽입 규칙
				// ----------------------------
				if (typeid(T) == typeid(int)
					|| typeid(T) == typeid(unsigned int)
					|| typeid(T) == typeid(__int64)
					|| typeid(T) == typeid(unsigned __int64))
				{
					// 정수 계열:
					// - bitPos==1일 때: 최상위 1비트(부호 위치) 뒤 구분
					// - 그 외: 4비트(니블) 단위로 구분
					if (bitPos == 1) {
						printf(" ");
					}
					else if (bitPos > 0 && ((j + 1) % 4 == 0)) {
						printf(" ");
					}
				}
				else if (typeid(T) == typeid(float))
				{
					// float(32bit) = sign(1) + exponent(8) + mantissa(23)
					// - bitPos==1: sign 뒤
					// - bitPos==9: exponent(8bit) 뒤 (1 + 8 = 9)
					if (bitPos == 1 || bitPos == 9) {
						printf(" ");
					}
					else if (bitPos > 9 && ((bitPos - 9) % 5 == 0)) {
						// mantissa 가독성용 5비트 그룹
						printf(" ");
					}
				}
				else if (typeid(T) == typeid(double))
				{
					// double(64bit) = sign(1) + exponent(11) + mantissa(52)
					// - bitPos==1 : sign 뒤
					// - bitPos==12: exponent(11bit) 뒤 (1 + 11 = 12)
					if (bitPos == 1 || bitPos == 12) {
						printf(" ");
					}
					else if (bitPos > 12 && ((bitPos - 12) % 5 == 0)) {
						// mantissa 가독성용 5비트 그룹
						printf(" ");
					}
				}

				// 현재 비트 출력
				printf("%u", (unsigned)bit);
			}
		}
		puts("");
	}
	
	// FP2BIN_STRING_MAX:
	//   - double에서 극단적으로 작은 subnormal(2^-1074) 같은 경우까지 고려하면
	//     소수부 비트가 매우 길어질 수 있어 큰 버퍼를 잡아둔 것입니다.
	//   - 다만 아래 fp2bin_f는 반복 종료 조건이 fp_frac > 0이라,
	//     0.1 같은 무한 이진 소수는 무한 루프가 될 수 있습니다.
	//     => 실전/학습용으로도 "최대 자릿수 제한"을 추가하는 것을 권장합니다.
	#define FP2BIN_STRING_MAX 1074

	void fp2bin_i(double fp_int, char* binString)
	{
		/*
			📚 정수부를 2진 문자열로 변환
			
			  - 2로 나누면서 나머지(0/1)를 얻어 역순으로 저장
			  - 마지막에 문자열을 뒤집어서 정방향 2진 문자열 완성
		*/

		int bitCount = 0;
		char binString_temp[FP2BIN_STRING_MAX];

		// fp_int가 0보다 큰 동안 나머지를 뽑아 저장
		// fmod(fp_int, 2)는 (fp_int % 2)와 유사한 역할
		do {
			binString_temp[bitCount++] = '0' + (int)fmod(fp_int, 2);
			fp_int = floor(fp_int / 2);
		} while (fp_int > 0);

		// 역순으로 쌓였으므로 뒤집어서 최종 문자열로
		for (int i = 0; i < bitCount; ++i) {
			binString[i] = binString_temp[bitCount - i - 1];
		}

		binString[bitCount] = 0; // 문자열 종료
	}

	void fp2bin_f(double fp_frac, char* binString)
	{
		/*
			📚 소수부를 2진 문자열로 변환

			  - 소수부에 2를 곱한다.
			  - 그 결과의 정수부가 1이면 다음 비트는 1, 0이면 다음 비트는 0.
			  - 정수부를 떼고 남은 소수부로 반복.

			  ★주의:
			    - 0.1 같은 값은 이진수로 유한하게 끝나지 않습니다(무한 반복 소수).
			    - 또한 부동소수점 오차 때문에 0이 정확히 되지 않아 while(fp_frac > 0)이
			      사실상 끝나지 않을 수 있습니다.
			      => 반드시 최대 길이 제한을 두는 것이 안전합니다.
		*/

		int bitCount = 0;
		double fp_int = 0.0;

		while (fp_frac > 0)
		{
			fp_frac *= 2;

			// modf는 fp_frac를 (정수부 fp_int) + (소수부 반환값)으로 분리
			fp_frac = modf(fp_frac, &fp_int);

			// 정수부가 1이면 다음 비트는 '1', 0이면 '0'
			binString[bitCount++] = '0' + (int)fp_int;

			// (안전장치 없음) -> 실제로는 bitCount 제한이 필요
		}

		binString[bitCount] = 0; // 문자열 종료
	}

	void fp2bin(double fp, __out std::string& s)
	{
		/*
			📚 실수를 "정수부.소수부" 2진 문자열로 변환

			  - 먼저 NaN/INF/denormal 같은 특수값은 변환하지 않고 설명 문자열만 반환
			  - modf로 정수부(fp_int) / 소수부(fp_frac) 분리
			  - 정수부를 fp2bin_i로 변환 (없으면 "0")
			  - '.' 추가
			  - 소수부를 fp2bin_f로 변환 (없으면 "0")

			  ★주의:
				- fp2bin_f가 무한 루프 될 수 있으므로,
				  실제 사용 시에는 “최대 소수 비트 수” 제한을 넣는 것을 권장합니다.
		*/

		char bitList[FP2BIN_STRING_MAX];

		// (1) 특수값 필터링
		std::string strResult;
		int fp_result = checkFPClassWin32(fp, __out strResult);

		if (   _FPCLASS_PINF == fp_result
			|| _FPCLASS_NINF == fp_result
			|| _FPCLASS_PD == fp_result
			|| _FPCLASS_ND == fp_result
			|| _FPCLASS_QNAN == fp_result
			|| _FPCLASS_SNAN == fp_result)
		{
			s = strResult; // 변환 대신 설명 문자열 반환
			return;
		}

		// (2) 정수부/소수부 분리
		double fp_int = 0.0, fp_frac = 0.0;
		fp_frac = modf(fp, &fp_int);

		// (3) 정수부 변환
		if (fp_int != 0) {
			fp2bin_i(fp_int, bitList);
		}
		else {
			strcpy_s(bitList, "0");
		}

		// (4) 소수점 추가
		strcat_s(bitList, FP2BIN_STRING_MAX, ".");

		// (5) 소수부 변환
		if (fp_frac != 0) {
			fp2bin_f(fp_frac, bitList + strlen(bitList)); // 뒤에 이어 붙임
		}
		else {
			strcat_s(bitList, FP2BIN_STRING_MAX, "0");
		}

		// (6) 결과 저장
		s = bitList;
	}	

	void fundamental_data_types()
	{
		/*
			📚 Fundamental data types (기본 데이터 타입)

			---------------------------------------------------------------------------------------
			[1] 메모리와 타입의 관계
			---------------------------------------------------------------------------------------
			- 변수의 값은 컴퓨터 메모리 어딘가에 0과 1(비트)로 저장된다.
			- 프로그램은 그 값이 “어디에 저장되어 있는지(주소)”를 몰라도 된다.
			  => 변수 이름으로 참조하면 된다.
			- 하지만 프로그램이 반드시 알아야 하는 것은 “어떤 종류의 데이터인가(타입)”이다.
			  예) 정수, 문자, 큰 실수(부동소수점) 등은
				  모두 0/1로 저장되지만 해석 방식이 다르고,
				  보통 메모리 사용량도 서로 다르다.

			---------------------------------------------------------------------------------------
			[2] 기본 데이터 타입(Fundamental types)이란?
			---------------------------------------------------------------------------------------
			- C++ 언어가 직접 제공하는 가장 기본적인 타입들.
			- 대부분의 시스템이 네이티브로 다루는 “기본 저장 단위”를 표현한다.
			- 크게 다음 그룹으로 분류할 수 있다.

			  1) 문자 타입(Character types)
				 - 한 글자(예: 'A', '$')를 표현
				 - 가장 기본은 char (정확히 1바이트)
				 - 더 넓은 문자 집합을 위한 타입들도 존재

			  2) 정수 타입(Integer types)
				 - 7, 1024 같은 “정수(whole number)”를 저장
				 - 크기가 여러 가지이며, signed/unsigned로 나뉜다
				   * signed  : 음수/양수 모두 가능
				   * unsigned: 0 이상의 값만 가능

			  3) 부동소수점 타입(Floating-point types)
				 - 3.14, 0.01 같은 “실수(real number)”를 표현
				 - float / double / long double에 따라 정밀도가 달라진다

			  4) 불리언 타입(Boolean type)
				 - bool: true / false 두 상태만 표현

			---------------------------------------------------------------------------------------
			[3] C++ 기본 타입 전체 목록(요약)
			---------------------------------------------------------------------------------------
			1) 문자 타입
			   - char       : 정확히 1바이트(최소 8비트)
			   - char16_t   : char보다 작지 않음(최소 16비트)
			   - char32_t   : char16_t보다 작지 않음(최소 32비트)
			   - wchar_t    : 더 큰 문자 집합 표현용(크기는 구현/플랫폼 의존)

			2) 정수 타입(부호 있음: signed)
			   - signed char
			   - signed short int
			   - signed int
			   - signed long int
			   - signed long long int

			3) 정수 타입(부호 없음: unsigned)
			   - unsigned char
			   - unsigned short int
			   - unsigned int
			   - unsigned long int
			   - unsigned long long int
			   * (각각의 크기는 signed 대응 타입과 동일)

			4) 부동소수점 타입
			   - float
			   - double      : 정밀도는 float 이상
			   - long double : 정밀도는 double 이상

			5) 기타
			   - bool
			   - void              : 저장 공간 없음(값/타입이 “없음”을 의미)
			   - decltype(nullptr) : 널 포인터 전용 타입

			---------------------------------------------------------------------------------------
			[4] 정수 타입 이름은 축약 가능
			---------------------------------------------------------------------------------------
			- 일부 정수 타입은 signed / int를 생략해도 같은 타입을 의미한다.
			  예)
				signed short int == signed short == short int == short

			---------------------------------------------------------------------------------------
			[5] 같은 그룹 내에서의 차이 = 주로 크기(size)
			---------------------------------------------------------------------------------------
			- 각 그룹에서 “첫 타입이 가장 작고, 마지막이 가장 크다”
			- 뒤에 오는 타입은 앞 타입보다 작지 않다(= 최소한 동일하거나 더 크다)
			- 같은 그룹의 타입들은 기본 성질이 유사하고, 주로 크기/범위가 다르다

			---------------------------------------------------------------------------------------
			[6] char를 제외하면 “정확한 크기”는 표준이 고정하지 않는다
			---------------------------------------------------------------------------------------
			- char는 정확히 1바이트로 고정.
			- 그 외 기본 타입들은 “최소 크기만 보장”한다.
			  => 플랫폼/컴파일러/아키텍처에 따라 실제 크기는 달라질 수 있다.
			- 이는 C++이 다양한 환경에서 최적 동작하도록 하기 위한 유연성이다.

			---------------------------------------------------------------------------------------
			[7] 비트 수가 커지면?
			---------------------------------------------------------------------------------------
			- 비트 수↑ => 표현 가능한 값의 개수(범위)↑, 하지만 메모리 사용량↑
			  예)
				8-bit  : 2^8  = 256
				16-bit : 2^16 = 65,536
				32-bit : 2^32 = 4,294,967,296 (약 40억)
				64-bit : 2^64 (매우 큼)

			---------------------------------------------------------------------------------------
			[8] signed vs unsigned 범위
			---------------------------------------------------------------------------------------
			- unsigned는 0 이상만 표현하지만 양수 범위가 더 넓다.
			- signed는 부호 비트(sign bit)가 필요해서 양수 범위가 대략 절반 정도 된다.
			- “양수 범위가 조금 더 크다”는 이유만으로 unsigned를 쓰는 것은
			  실무에서 큰 근거가 되지 않는 경우가 많다(부호 관련 버그 주의).

			---------------------------------------------------------------------------------------
			[9] 부동소수점에서 크기 = 정밀도
			---------------------------------------------------------------------------------------
			- float / double / long double은 크기가 커질수록
			  가수/지수에 쓸 수 있는 비트가 늘어 정밀도가 높아진다.

			---------------------------------------------------------------------------------------
			[10] 보통의 기본 선택(특별한 이유가 없다면)
			---------------------------------------------------------------------------------------
			- 문자: char
			- 정수: int
			- 실수: double
			- 그 외 타입들은 특정 목적(메모리/범위/정밀도/호환성 등)일 때 선택

			---------------------------------------------------------------------------------------
			[11] 시스템별 타입 정보 확인 방법
			---------------------------------------------------------------------------------------
			- <limits>의 std::numeric_limits<T> 로 범위/특성을 확인할 수 있다.
			- 특정 비트 폭이 꼭 필요하면 <cstdint>의 고정 크기 타입을 사용한다.
			  예) int32_t, uint64_t 등

			---------------------------------------------------------------------------------------
			[12] 산술 타입(arithmetic types)과 추가 기본 타입
			---------------------------------------------------------------------------------------
			- 문자/정수/실수/bool은 묶어서 산술 타입(arithmetic types)이라 부른다.
			- 추가 기본 타입:
			  * void     : 타입 없음
			  * nullptr  : 널 포인터를 위한 특수 타입
			- 포인터 챕터에서 보통 더 자세히 다룬다.

			---------------------------------------------------------------------------------------
			[13] 복합 타입(compound types)
			---------------------------------------------------------------------------------------
			- C++은 기본 타입을 기반으로 포인터/참조/배열/구조체/클래스 등
			  다양한 “복합 타입”을 만들 수 있고, 이것이 C++의 큰 강점이다.
		*/
		{
			//size of types
			{
				printf("char : %zu byte\n", sizeof(char));
				printf("short : %zu byte\n", sizeof(short));
				printf("int : %zu byte\n", sizeof(int));
				printf("long int : %zu byte\n", sizeof(long int));
				printf("long long int : %zu byte\n", sizeof(long long int));
				printf("float : %zu byte\n", sizeof(float));
				printf("double : %zu byte\n", sizeof(double));
				printf("long double : %zu byte\n", sizeof(long double));
				printf("\n");
				printf("(void*) : %zu byte\n", sizeof(void*));
				printf("(int*) : %zu byte\n", sizeof(int*));
				printf("\n");

				system("pause");

				/*
				output:
					char : 1 byte
					short : 2 byte
					int : 4 byte
					long int : 4 byte
					long long int : 8 byte
					float : 4 byte
					double : 8 byte
					long double : 8 byte

					(void*) : 8 byte
					(int*) : 8 byte
				*/

				system("pause");
			}

			//bit of types
			{
				std::string bitString;

				printBitFormatOfType(1.0f);
				printBitFormatOfType(-1.0f);
				/*
				output:
					1.000000 : float = 0 01111111 00000 00000 00000 00000 000
					-1.000000 : float = 1 01111111 00000 00000 00000 00000 000
				*/

				printBitFormatOfType(10.0f);
				printBitFormatOfType(-10.0f);
				/*
				output:
					10.000000 : float = 0 10000010 01000 00000 00000 00000 000
					-10.000000 : float = 1 10000010 01000 00000 00000 00000 000
				*/

				printBitFormatOfType(100.0f);
				printBitFormatOfType(-100.0f);
				/*
				output:
					100.000000 : float = 0 10000101 10010 00000 00000 00000 000
					-100.000000 : float = 1 10000101 10010 00000 00000 00000 000
				*/

				printBitFormatOfType(1000.0f);
				printBitFormatOfType(-1000.0f);
				/*
				output:
					1000.000000 : float = 0 10001000 11110 10000 00000 00000 000
					-1000.000000 : float = 1 10001000 11110 10000 00000 00000 000
				*/

				printBitFormatOfType(10000.0f);
				printBitFormatOfType(-10000.0f);
				/*
				output:
					10000.000000 : float = 0 10001100 00111 00010 00000 00000 000
					-10000.000000 : float = 1 10001100 00111 00010 00000 00000 000
				*/

				printBitFormatOfType(0.1f);
				printBitFormatOfType(-0.1f);
				/*
				output:
					0.100000 : float = 0 01111011 10011 00110 01100 11001 101
					-0.100000 : float = 1 01111011 10011 00110 01100 11001 101
				*/

				printBitFormatOfType(0.01f);
				printBitFormatOfType(-0.01f);
				/*
				output:
					0.010000 : float = 0 01111000 01000 11110 10111 00001 010
					-0.010000 : float = 1 01111000 01000 11110 10111 00001 010
				*/

				printBitFormatOfType(0.001f);
				printBitFormatOfType(-0.001f);
				/*
				output:
					0.001000 : float = 0 01110101 00000 11000 10010 01101 111
					-0.001000 : float = 1 01110101 00000 11000 10010 01101 111
				*/

				system("pause");
			}

			// [1] 메모리와 타입의 관계: 같은 “0/1”이라도 타입에 따라 해석이 달라짐
			{
				printLine("[1] Memory & Type: same bits, different meaning");

				// 같은 32비트를 int와 float로 해석해보기(유니온/bit-copy)
				uint32_t bits = 0x3F800000u; // float 1.0f의 IEEE-754 비트 패턴으로 널리 알려진 값
				float f = 0.0f;
				int i = 0;

				static_assert(sizeof(bits) == sizeof(f), "uint32_t and float must be 4 bytes");
				static_assert(sizeof(bits) == sizeof(i), "uint32_t and int must be 4 bytes");

				memcpy(&f, &bits, sizeof(bits));
				memcpy(&i, &bits, sizeof(bits));

				std::cout << "raw bits (hex) = 0x" << std::hex << bits << std::dec << "\n";
				std::cout << "as float       = " << f << "\n";
				std::cout << "as int         = " << i << "\n";
				std::cout << "(같은 비트라도 타입이 달라지면 값의 해석이 완전히 달라짐)\n";

				system("pause");
			}

			// [2][3] 기본 타입 분류/목록: sizeof로 크기 확인
			{
				printLine("[2][3] Fundamental types: sizeof (platform dependent except char=1 byte)");

				std::cout << "sizeof(char)        = " << sizeof(char) << "\n";
				std::cout << "sizeof(char16_t)    = " << sizeof(char16_t) << "\n";
				std::cout << "sizeof(char32_t)    = " << sizeof(char32_t) << "\n";
				std::cout << "sizeof(wchar_t)     = " << sizeof(wchar_t) << "\n\n";

				std::cout << "sizeof(signed char) = " << sizeof(signed char) << "\n";
				std::cout << "sizeof(short)       = " << sizeof(short) << "\n";
				std::cout << "sizeof(int)         = " << sizeof(int) << "\n";
				std::cout << "sizeof(long)        = " << sizeof(long) << "\n";
				std::cout << "sizeof(long long)   = " << sizeof(long long) << "\n\n";

				std::cout << "sizeof(unsigned int)= " << sizeof(unsigned int) << "\n\n";

				std::cout << "sizeof(float)       = " << sizeof(float) << "\n";
				std::cout << "sizeof(double)      = " << sizeof(double) << "\n";
				std::cout << "sizeof(long double) = " << sizeof(long double) << "\n\n";

				std::cout << "sizeof(bool)        = " << sizeof(bool) << "\n";
				std::cout << "sizeof(void*)       = " << sizeof(void*) << "\n";
				std::cout << "sizeof(nullptr)     = " << sizeof(decltype(nullptr)) << "\n";

				// 표준 보장: char는 반드시 1바이트
				static_assert(sizeof(char) == 1, "C++ standard: sizeof(char) is always 1 byte");

				system("pause");
			}

			// [4] 정수 타입 이름 축약: 타입 동치성(static_assert로 확인)
			{	
				printLine("[4] Integer type name abbreviation equivalence");

				static_assert(std::is_same<short, short int>::value, "short == short int");
				static_assert(std::is_same<signed short, signed short int>::value, "signed short == signed short int");
				static_assert(std::is_same<int, signed int>::value, "int == signed int");

				std::cout << "static_assert로 short/short int 등이 같은 타입임을 확인했습니다.\n";

				system("pause");
			}

			// [5] 같은 그룹 내 차이 = 주로 크기: sizeof 관계 출력
			{
				printLine("[5] Within-group difference is mainly size (non-decreasing)");

				std::cout << "sizeof(char)  <= sizeof(short) ? " << (sizeof(char) <= sizeof(short)) << "\n";
				std::cout << "sizeof(short) <= sizeof(int)   ? " << (sizeof(short) <= sizeof(int)) << "\n";
				std::cout << "sizeof(int)   <= sizeof(long)  ? " << (sizeof(int) <= sizeof(long)) << "\n";
				std::cout << "sizeof(long)  <= sizeof(long long) ? " << (sizeof(long) <= sizeof(long long)) << "\n";

				std::cout << "(표준은 최소 크기만 보장하므로, 관계는 플랫폼에 따라 같거나 커질 수 있음)\n";

				system("pause");
			}

			// [6] char 제외 크기 고정 아님: 현재 플랫폼 값 출력
			{
				printLine("[6] Sizes are not fixed (except char). Show current platform result");

				std::cout << "This platform sizes:\n";
				std::cout << "  char      : " << sizeof(char) << "\n";
				std::cout << "  short     : " << sizeof(short) << "\n";
				std::cout << "  int       : " << sizeof(int) << "\n";
				std::cout << "  long      : " << sizeof(long) << "\n";
				std::cout << "  long long : " << sizeof(long long) << "\n";
				std::cout << "  void*     : " << sizeof(void*) << "\n";
				std::cout << "(예: Windows/MSVC 64-bit는 보통 long=4)\n";

				system("pause");
			}

			// [7] 비트 수↑ => 2^N 출력(64는 overflow 주의)
			{
				printLine("[7] Bits -> unique representable values (2^N)");

				std::cout << "8-bit  : " << (1ULL << 8) << " = 2^8\n";
				std::cout << "16-bit : " << (1ULL << 16) << " = 2^16\n";
				std::cout << "32-bit : " << (1ULL << 32) << " = 2^32\n";
				std::cout << "64-bit : 2^64 (uint64_t 범위를 넘어가므로 숫자 출력은 생략)\n";

				system("pause");
			}

			// [8] signed vs unsigned 범위: numeric_limits
			{
				printLine("[8] signed vs unsigned ranges");

				std::cout << "int min/max           = "
					<< std::numeric_limits<int>::min() << " / " << std::numeric_limits<int>::max() << "\n";
				std::cout << "unsigned int min/max  = "
					<< std::numeric_limits<unsigned int>::min() << " / " << std::numeric_limits<unsigned int>::max() << "\n";

				std::cout << "short min/max         = "
					<< std::numeric_limits<short>::min() << " / " << std::numeric_limits<short>::max() << "\n";
				std::cout << "unsigned short min/max= "
					<< std::numeric_limits<unsigned short>::min() << " / " << std::numeric_limits<unsigned short>::max() << "\n";

				std::cout << "(unsigned는 0부터 시작, signed는 음수 포함 + 양수 범위가 대략 절반)\n";

				system("pause");
			}

			// [9] float vs double 정밀도: digits10/epsilon + 0.1 누적
			{
				printLine("[9] Floating-point precision (float vs double)");

				std::cout << "float  digits10  = " << std::numeric_limits<float>::digits10 << "\n";
				std::cout << "double digits10  = " << std::numeric_limits<double>::digits10 << "\n";
				std::cout << "float  epsilon   = " << std::numeric_limits<float>::epsilon() << "\n";
				std::cout << "double epsilon   = " << std::numeric_limits<double>::epsilon() << "\n";

				float fs = 0.0f;
				double ds = 0.0;
				for (int k = 0; k < 10; ++k) { fs += 0.1f; ds += 0.1; }

				std::cout << "float  sum(0.1f x10) = " << fs << "\n";
				std::cout << "double sum(0.1  x10) = " << ds << "\n";

				system("pause");
			}

			// [10] 보통 선택: char/int/double 예시
			{
				printLine("[10] Typical choices: char / int / double");

				char ch = 'A';
				int count = 1024;
				double pi = 3.141592653589793;

				std::cout << "char   ch    = " << ch << "\n";
				std::cout << "int    count = " << count << "\n";
				std::cout << "double pi    = " << pi << "\n";

				system("pause");
			}

			// [11] numeric_limits & <cstdint> 고정폭 타입
			{
				printLine("[11] numeric_limits + <cstdint> fixed-size types");

				std::cout << "int32_t  size = " << sizeof(int32_t) << " bytes\n";
				std::cout << "uint64_t size = " << sizeof(uint64_t) << " bytes\n";

				std::cout << "int32_t  min/max = "
					<< std::numeric_limits<int32_t>::min() << " / " << std::numeric_limits<int32_t>::max() << "\n";
				std::cout << "uint64_t min/max = "
					<< std::numeric_limits<uint64_t>::min() << " / " << std::numeric_limits<uint64_t>::max() << "\n";

				system("pause");
			}

			// [12] arithmetic types + void/nullptr (v140: is_arithmetic_v 없음)
			{
				printLine("[12] arithmetic types + void + nullptr");

				static_assert(std::is_arithmetic<int>::value, "int is arithmetic");
				static_assert(std::is_arithmetic<double>::value, "double is arithmetic");
				static_assert(std::is_arithmetic<char>::value, "char is arithmetic");
				static_assert(std::is_arithmetic<bool>::value, "bool is arithmetic");

				std::cout << "static_assert로 산술 타입(arithmetic types)임을 확인했습니다.\n";

				void* p = nullptr;
				std::cout << "void* p = " << p << " (nullptr)\n";

				decltype(nullptr) np = nullptr;
				std::cout << "decltype(nullptr) size = " << sizeof(np) << "\n";

				system("pause");
			}

			// [13] compound types: 배열/포인터/참조/구조체
			{
				printLine("[13] Compound types examples (array/pointer/reference/struct)");

				int arr[3] = { 10, 20, 30 };

				int* p = &arr[0];
				int& r = arr[1];

				struct Player
				{
					int id;
					float hp;
					bool alive;
				};

				Player pl = { 7, 99.5f, true };

				std::cout << "array arr[0..2] = " << arr[0] << ", " << arr[1] << ", " << arr[2] << "\n";
				std::cout << "pointer p -> *p = " << *p << "\n";
				std::cout << "reference r (arr[1]) = " << r << "\n";
				r = 777;
				std::cout << "after r=777, arr[1] = " << arr[1] << "\n";

				std::cout << "struct Player = { id=" << pl.id << ", hp=" << pl.hp << ", alive=" << pl.alive << " }\n";

				system("pause");
			}
		}
	}

	//---------------------------------------------------------------------------------------------

	bool safe_add_u32(unsigned __int32 a, unsigned __int32 b, unsigned __int32& out)
	{
		const unsigned __int32 max = std::numeric_limits<unsigned __int32>::max();

		// unsigned는 a+b가 max를 넘으면 랩어라운드
		if (b > max - a) return false;

		out = a + b;
		return true;
	}

	bool safe_sub_u32(unsigned __int32 a, unsigned __int32 b, unsigned __int32& out)
	{
		// a-b에서 b > a면 랩어라운드(언더플로우 느낌)
		if (b > a) return false;

		out = a - b;
		return true;
	}

	bool safe_add_i32(__int32 a, __int32 b, __int32& out)
	{
		const __int32 max = std::numeric_limits<__int32>::max();
		const __int32 min = std::numeric_limits<__int32>::min();

		// b의 부호에 따라 검사식이 달라짐(이게 정석)
		if (b > 0)
		{
			if (a > max - b) return false;
		}
		else if (b < 0)
		{
			if (a < min - b) return false;
		}

		out = a + b;
		return true;
	}

	bool safe_sub_i32(__int32 a, __int32 b, __int32& out)
	{
		const __int32 max = std::numeric_limits<__int32>::max();
		const __int32 min = std::numeric_limits<__int32>::min();

		// a - b == a + (-b) 로 생각하면 동일하게 처리 가능
		// 단, -b가 overflow 나는 케이스(b==INT_MIN)는 별도 처리 필요
		if (b == min)
		{
			// a - INT_MIN 은 a + 2147483648 이 되어 범위를 벗어날 수 있음
			// (INT_MIN의 절댓값은 int에 담기지 않음)
			// 안전하게 판정:
			// a - min => 항상 overflow(양수 방향) 가능성이 매우 큼.
			// 정확히는 a가 음수여도 결과가 max를 넘어갈 수 있음.
			// a - min = a + 2147483648 이므로 int32 범위 밖 -> 무조건 실패 처리
			return false;
		}

		__int32 nb = -b;
		return safe_add_i32(a, nb, out);
	}

	void integer_over_under_flow()
	{
		// 1) unsigned 32-bit add
		{
			unsigned __int32 a = std::numeric_limits<unsigned __int32>::max();
			unsigned __int32 b = 1;
			unsigned __int32 r = 0;

			if (!safe_add_u32(a, b, r))
				printf("Overflow(=wrap) Unsigned 32-bit add !!! : a=%u, b=%u\n", a, b);
			else
				printf("Unsigned 32-bit add ok : a=%u, b=%u, r=%u\n", a, b, r);
		}

		// 2) unsigned 32-bit sub
		{
			unsigned __int32 a = std::numeric_limits<unsigned __int32>::min(); // 0
			unsigned __int32 b = 1;
			unsigned __int32 r = 0;

			if (!safe_sub_u32(a, b, r))
				printf("Underflow(=wrap) Unsigned 32-bit sub !!! : a=%u, b=%u\n", a, b);
			else
				printf("Unsigned 32-bit sub ok : a=%u, b=%u, r=%u\n", a, b, r);
		}

		// 3) signed 32-bit add
		{
			__int32 a = std::numeric_limits<__int32>::max();
			__int32 b = 1;
			__int32 r = 0;

			if (!safe_add_i32(a, b, r))
				printf("Overflow Signed 32-bit add !!! : a=%d, b=%d\n", a, b);
			else
				printf("Signed 32-bit add ok : a=%d, b=%d, r=%d\n", a, b, r);
		}

		// 4) signed 32-bit sub
		{
			__int32 a = std::numeric_limits<__int32>::min();
			__int32 b = 1;
			__int32 r = 0;

			if (!safe_sub_i32(a, b, r))
				printf("Underflow Signed 32-bit sub !!! : a=%d, b=%d\n", a, b);
			else
				printf("Signed 32-bit sub ok : a=%d, b=%d, r=%d\n", a, b, r);
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void declaration_of_variables()
	{
		/*
			📚 변수의 선언 (Declaration of variables)

			C++는 강한 형식(Strongly-typed) 언어이기 때문에,
			변수를 사용하기 전에 반드시 먼저
			"어떤 자료형인지" 선언해야 한다.

			즉 컴파일러에게:
			1) 이 변수가 메모리에서 얼마나 큰 공간이 필요한지
			2) 이 값을 어떤 타입으로 해석해야 하는지
			를 알려줘야 한다.

			변수 선언 문법은 매우 단순하다.

				자료형 변수이름;

			예:
				int a;
				float mynumber;

			같은 자료형 변수 여러 개를 한 줄에 선언할 수도 있다.

				int a, b, c;

			이것은 아래와 같은 의미이다.

				int a;
				int b;
				int c;
		*/

		{
			// 변수 선언
			float number;   // 실수형 변수
			int a, b;       // 정수형 변수 2개
			int result;     // 정수형 변수 1개

			// 값 대입 및 계산
			a = 5;
			b = 2;
			a = a + 1;      // a는 6이 됨
			result = a - b; // 6 - 2 = 4

			std::cout << result << std::endl;

			/*
				출력 결과:
					4
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 같은 자료형 여러 변수 선언" << std::endl;
			std::cout << "============================================" << std::endl;

			int x, y, z;

			x = 10;
			y = 20;
			z = x + y;

			std::cout << "x = " << x << std::endl;
			std::cout << "y = " << y << std::endl;
			std::cout << "z = " << z << std::endl;
			std::cout << std::endl;

			/*
				설명:
				int x, y, z;
				는 int형 변수 3개를 한 번에 선언한 것이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 선언만 하고 아직 값이 없는 상태" << std::endl;
			std::cout << "============================================" << std::endl;

			int value;

			std::cout << "value 변수는 선언되었지만 아직 값을 직접 넣지 않았다." << std::endl;
			std::cout << "선언 후에는 반드시 값을 넣고 사용하는 습관이 중요하다." << std::endl;
			std::cout << std::endl;

			/*
				중요:
				지역 변수는 선언만 하면 초기값이 자동으로 보장되지 않는다.
				따라서 사용 전에 반드시 값을 넣는 것이 안전하다.
			*/
		}

		system("pause");
	}

	void initialization_of_variables()
	{
		/*
			📚 변수의 초기화 (Initialization of variables)

			변수를 선언만 하면,
			그 변수는 처음 값을 대입하기 전까지는
			의미 있는 값이 들어 있다고 보장되지 않는다.

			그래서 변수를 선언하는 순간
			바로 원하는 값을 넣어주는 것을
			초기화(initialization)라고 한다.

			C++에서는 여러 가지 초기화 문법이 있다.

			대표적으로:
			1) C 스타일 초기화
				int x = 0;

			2) 생성자 형태 초기화
				int x(0);

			둘 다 유효하다.
		*/

		{
			int x = 0;
			int y(0);

			int a = 5;   // 선언과 동시에 5로 초기화
			int b(3);    // 선언과 동시에 3으로 초기화
			int result;  // 아직 초기화하지 않음

			a = a + b;   // 5 + 3 = 8
			result = a - b; // 8 - 3 = 5

			std::cout << result << std::endl;

			/*
				출력 결과:
					5
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 선언만 한 변수와 초기화한 변수 비교" << std::endl;
			std::cout << "============================================" << std::endl;

			int initializedValue = 100;
			int anotherValue(200);
			int laterAssigned;

			laterAssigned = 300;

			std::cout << "initializedValue = " << initializedValue << std::endl;
			std::cout << "anotherValue     = " << anotherValue << std::endl;
			std::cout << "laterAssigned    = " << laterAssigned << std::endl;
			std::cout << std::endl;

			/*
				설명:
				initializedValue 와 anotherValue 는
				선언과 동시에 값이 정해졌다.

				laterAssigned 는 선언 후 나중에 값을 넣었다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 초기화가 중요한 이유" << std::endl;
			std::cout << "============================================" << std::endl;

			int score = 0;

			std::cout << "초기 score = " << score << std::endl;

			score = score + 10;
			std::cout << "변경 후 score = " << score << std::endl;
			std::cout << std::endl;

			/*
				설명:
				초기값을 명확히 주면
				나중에 계산할 때 훨씬 안전하고 예측 가능하다.
			*/
		}

		system("pause");
	}

	void type_deduction_auto_n_decltype()
	{
		/*
			📚 타입 추론 : auto 와 decltype

			C++에서는 어떤 경우
			컴파일러가 초기값을 보고 변수의 타입을 자동으로 추론할 수 있다.

			1) auto
				초기값을 기준으로 타입을 자동 결정한다.

				예:
					int foo = 0;
					auto bar = foo;   // bar는 int

			2) decltype
				어떤 식(expression) 또는 변수의 타입을 그대로 가져온다.

				예:
					int foo = 0;
					decltype(foo) bar;   // bar는 int

			주의:
			auto 는 반드시 초기값이 있어야 타입을 추론할 수 있다.
			반면 decltype 은 초기값 없이도 특정 식의 타입을 그대로 가져올 수 있다.

			또한 auto 와 decltype 은 편리하지만,
			무분별하게 쓰면 오히려 코드 가독성이 떨어질 수 있다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] auto 기본 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int foo = 0;
			auto bar = foo;  // bar는 int로 추론됨

			bar = 100;

			std::cout << "foo = " << foo << std::endl;
			std::cout << "bar = " << bar << std::endl;
			std::cout << std::endl;

			/*
				설명:
				bar는 auto 이지만,
				초기값 foo가 int 이므로
				bar도 int가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] auto로 실수 타입 추론" << std::endl;
			std::cout << "============================================" << std::endl;

			auto a = 3.14;   // double로 추론
			auto b = 10;     // int로 추론
			auto c = 'A';    // char로 추론

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "c = " << c << std::endl;
			std::cout << std::endl;

			/*
				설명:
				auto는 오른쪽 초기값의 타입을 보고
				자동으로 타입을 정한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] decltype 기본 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int foo = 123;
			decltype(foo) bar; // bar는 int

			bar = 456;

			std::cout << "foo = " << foo << std::endl;
			std::cout << "bar = " << bar << std::endl;
			std::cout << std::endl;

			/*
				설명:
				decltype(foo) 는
				foo의 타입이 int 이므로
				bar를 int로 선언한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] decltype으로 식의 타입 가져오기" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 10;
			int y = 20;

			decltype(x + y) sum = x + y;

			std::cout << "sum = " << sum << std::endl;
			std::cout << std::endl;

			/*
				설명:
				x + y 의 결과 타입은 int 이므로
				sum도 int가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] 언제 쓰면 좋은가" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "auto / decltype 은 타입이 너무 길거나" << std::endl;
			std::cout << "표현식으로부터 타입을 자연스럽게 얻고 싶을 때 유용하다." << std::endl;
			std::cout << "하지만 단순한 int, double까지 무조건 auto로 쓰면" << std::endl;
			std::cout << "오히려 읽기 어려워질 수 있다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void introduction_to_strings()
	{
		/*
			📚 문자열 소개 (Introduction to strings)

			기본 자료형(int, float, char 등)은
			컴퓨터가 직접 다루는 가장 기초적인 값들이다.

			하지만 C++의 강점 중 하나는
			이 기본 자료형들을 바탕으로 더 풍부한 복합 타입(compound type)을 제공한다는 점이다.

			그 대표적인 예가 string 클래스이다.

			string 은 문자들의 연속,
			즉 단어나 문장 같은 문자열을 저장할 수 있는 타입이다.

			string 을 사용하려면
			표준 라이브러리의 <string> 헤더가 필요하다.
		*/

		{
			std::string myString;
			myString = "This is a string";
			std::cout << myString << std::endl;

			/*
				출력 결과:
					This is a string
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 문자열 초기화 방법" << std::endl;
			std::cout << "============================================" << std::endl;

			std::string s1 = "This is a string";
			std::string s2("This is a string");
			std::string s3 = "Another string";

			std::cout << "s1 = " << s1 << std::endl;
			std::cout << "s2 = " << s2 << std::endl;
			std::cout << "s3 = " << s3 << std::endl;
			std::cout << std::endl;

			/*
				설명:
				string 도 기본 자료형처럼
				여러 방식으로 초기화할 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 문자열 값 변경" << std::endl;
			std::cout << "============================================" << std::endl;

			std::string myString;
			myString = "This is the initial string content";
			std::cout << myString << std::endl;

			myString = "This is a different string content";
			std::cout << myString << std::endl;
			std::cout << std::endl;

			/*
				출력:
					This is the initial string content
					This is a different string content

				설명:
				string 변수도 일반 변수처럼
				실행 중에 값을 바꿀 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 문자열 연결" << std::endl;
			std::cout << "============================================" << std::endl;

			std::string firstName = "Justin";
			std::string lastName = "Kang";
			std::string fullName = firstName + " " + lastName;

			std::cout << "fullName = " << fullName << std::endl;
			std::cout << std::endl;

			/*
				설명:
				string 은 + 연산자를 사용해
				문자열을 이어 붙일 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] 문자열 길이 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			std::string text = "Hello";

			std::cout << "text = " << text << std::endl;
			std::cout << "length = " << text.length() << std::endl;
			std::cout << std::endl;

			/*
				설명:
				string 은 클래스이기 때문에
				길이 확인 같은 기능도 제공한다.
			*/
		}

		/*
			참고:
			std::endl 은 줄바꿈을 출력하고
			스트림을 비우는(flush) 역할도 한다.
		*/

		system("pause");
	}

	void volatile_what()
	{
		/*
			📚 volatile (C/C++)

			  - volatile은 변수(메모리)에 대한 컴파일러 최적화 제한을 의미하는 키워드
			    (즉, 항상 "실제 메모리"에서 값을 읽고 쓰도록 강제함)
			  - 용도:
				+ 하드웨어 IO, 인터럽트 서비스 루틴, 멀티스레드 환경에서
				  메모리 값이 코드 이외의 경로로 변경될 수 있는 변수에 사용
				+ 예: 플래그, 센서값, 레지스터, 외부 디바이스 메모리 등
			  - 효과:
				+ 컴파일러가 변수값을 레지스터 등에 캐싱하지 않고,
				  매번 메모리에서 접근하도록 하여 외부 변화에 대응
				+ 코드 순서 재배치 등 최적화도 제한
			  - 한계:
				+ 동기화, 원자성, 메모리 배리어를 보장하지 않음
				+ 멀티스레드 환경에서 데이터 경합, 순서, 원자성 보장은 별도의 동기화(mutex, atomic 등) 필요

			  ※ 현대 C++에서는 멀티스레드 데이터 보호에는 std::atomic, mutex 등 표준 동기화 도구 사용이 원칙
			  ※ volatile은 "최적화 억제"만 담당, 진짜 동기화 용도 아님을 항상 주의!
		*/

		// 1. volatile 없이 루프 내 계산 (컴파일러 최적화로 반복이 생략될 수 있음)
		{
			std::cout << "[일반 변수 루프 최적화 예제]" << std::endl;
			double sum = 0;
			for (int i = 0; i < 10; ++i) {
				sum = std::sqrt(2.8) + std::log(3.5) + 56; // 컴파일러가 반복 내 계산을 1회만 할 수도 있음
			}
			std::cout << "sum (no volatile): " << sum << std::endl;
			/*
			출력:
				[일반 변수 루프 최적화 예제]
				sum (no volatile): 58.0562	
			*/

			system("pause");
		}

		// 2. volatile 사용: 매번 메모리 접근을 강제, 반복 내 계산이 생략되지 않음
		{
			std::cout << "[volatile 변수 루프 최적화 방지 예제]" << std::endl;
			volatile double sum = 0;
			for (int i = 0; i < 10; ++i) {
				sum = std::sqrt(2.8) + std::log(3.5) + 56; // 반드시 10번 모두 계산됨
			}
			std::cout << "sum (volatile): " << sum << std::endl;
			/*
			출력:
				[volatile 변수 루프 최적화 방지 예제]
				sum (volatile): 58.0562
			*/

			system("pause");
		}

		// 3. volatile 변수의 "외부에서 변경 가능함" 의미 시뮬레이션
		{
			std::cout << "[volatile 변수 외부 변경 예제(시뮬레이션)]" << std::endl;
			volatile int flag = 1;
			int loop = 0;
			while (flag) {
				++loop;
				if (loop > 1000000) flag = 0; // 실제 환경에선 ISR, 다른 쓰레드, 하드웨어가 flag를 바꿔줌
			}
			std::cout << "Loop exited, loop count = " << loop << std::endl;
			/*
			출력:
				[volatile 변수 외부 변경 예제(시뮬레이션)]
				Loop exited, loop count = 1000001
			*/

			system("pause");
		}

		// 4. 순서 보장: volatile이 있을 때와 없을 때
		{
			std::cout << "[volatile로 코드 순서 보장 예제]" << std::endl;
			int a = 10;
			volatile int b = 20;
			int c = 30;
			// 컴파일러는 volatile 변수 앞뒤로 연산 순서를 바꾸지 않음(최적화 제한)
			std::cout << "a: " << a << ", b(volatile): " << b << ", c: " << c << std::endl;
			/*
			출력:
				[volatile로 코드 순서 보장 예제]
				a: 10, b(volatile): 20, c: 30

				비정상적으로 출력된 경우 => a: 10, b: 0, c: 30 
				                       또는 a: 10, b: 쓰레기값, c: 30
									   또는 a: 10, c: 30
			*/

			system("pause");
		}
	}

	void Test()
	{
		//volatile_what();

		//introduction_to_strings();

		//type_deduction_auto_n_decltype();

		//initialization_of_variables();

		//declaration_of_variables();

		//integer_over_under_flow();

		//identifiers();

		//variables_n_types();
	}

}// end of VariablesAndTypes