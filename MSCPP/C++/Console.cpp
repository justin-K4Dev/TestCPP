
#include "stdafx.h"


#include <Windows.h>


namespace Console
{
	// Linux / Unix 터미널 ANSI Escape Code 색상 매크로
	#define RESET		"\033[0m"				
	#define BLACK		"\033[30m"				/* Black */
	#define RED			"\033[31m"				/* Red */
	#define GREEN		"\033[32m"				/* Green */
	#define YELLOW		"\033[33m"				/* Yellow */
	#define BLUE		"\033[34m"				/* Blue */
	#define MAGENTA		"\033[35m"				/* Magenta */
	#define CYAN		"\033[36m"				/* Cyan */
	#define WHITE		"\033[37m"				/* White */
												
	#define BOLDBLACK   "\033[1m\033[30m"		/* Bold Black */
	#define BOLDRED     "\033[1m\033[31m"		/* Bold Red */
	#define BOLDGREEN   "\033[1m\033[32m"		/* Bold Green */
	#define BOLDYELLOW  "\033[1m\033[33m"		/* Bold Yellow */
	#define BOLDBLUE    "\033[1m\033[34m"		/* Bold Blue */
	#define BOLDMAGENTA "\033[1m\033[35m"		/* Bold Magenta */
	#define BOLDCYAN    "\033[1m\033[36m"		/* Bold Cyan */
	#define BOLDWHITE   "\033[1m\033[37m"		/* Bold White */

	#define CLEAR		"\033[2J"   // 화면 지우기 ANSI escape code


	enum Code 
	{
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_DEFAULT = 39,

		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};

	class Modifier
	{
	protected:
		Code code;

	public:
		Modifier(Code pCode) : code(pCode) { return; }

		friend std::ostream& operator<<(std::ostream& os, const Modifier& mod)
		{
			return os << "\033[" << mod.code << "m";
		}
	};

	void console_programs()
	{
		/*
			📚 콘솔 프로그램 (Console Programs)

			콘솔 프로그램은 텍스트 기반으로
			사용자와 상호작용하는 프로그램이다.

			즉:
				- 화면에 텍스트 출력
				- 키보드 입력 받기
				- 단순한 메뉴/로그/디버그 정보 표시

			같은 작업을 주로 수행한다.

			콘솔 프로그램은 GUI 프로그램보다 구조가 단순하고,
			기본 개념 학습에 매우 좋다.
			그래서 C++ 입문 예제 대부분은 콘솔 프로그램 형태로 시작한다.


			=======================================================================================
			1. 콘솔 출력 색상 처리 방식
			=======================================================================================

			콘솔에서 색상을 바꾸는 방식은 플랫폼마다 다르다.

			(1) Linux / Unix / macOS 터미널
				- ANSI Escape Code 사용

			(2) Windows 전통 콘솔
				- Win32 API 사용
				- SetConsoleTextAttribute()

			즉, 같은 "빨간 글자 출력"이라도
			플랫폼에 따라 구현 방식이 다를 수 있다.


			=======================================================================================
			2. ANSI Escape Code
			=======================================================================================

			Linux/Unix 계열 터미널에서는
			특수 문자열을 출력해 색상/커서/화면 제어를 할 수 있다.

			예:
				"\033[31m"
					빨간색 전경색

				"\033[0m"
					속성 초기화

				"\033[2J"
					화면 지우기

			이런 코드를 ANSI Escape Code 또는 ANSI Sequence 라고 한다.

			즉, 문자열만 출력해도
			터미널이 이를 해석해서 색상을 바꾸는 것이다.


			=======================================================================================
			3. Modifier 클래스 의미
			=======================================================================================

			Modifier 클래스는 ANSI 색상 코드를
			C++ 스트림 스타일로 쉽게 출력하기 위한 래퍼이다.

			예:
				Modifier red(FG_RED);
				Modifier def(FG_DEFAULT);

				std::cout << red << "text" << def;

			이렇게 하면
			색상 변경용 ANSI 코드를 직접 문자열로 쓰지 않아도 된다.


			=======================================================================================
			4. Win32 콘솔 색상
			=======================================================================================

			Windows 전통 콘솔에서는
			SetConsoleTextAttribute() 함수를 사용해
			콘솔 텍스트 속성을 바꾼다.

			대표 플래그:
				FOREGROUND_RED
				FOREGROUND_GREEN
				FOREGROUND_BLUE
				FOREGROUND_INTENSITY

			이들을 OR(|) 조합해서 원하는 색을 만든다.

			예:
				FOREGROUND_RED | FOREGROUND_GREEN
					빨강 + 초록

				FOREGROUND_RED | FOREGROUND_INTENSITY
					밝은 빨강


			=======================================================================================
			5. 주의사항
			=======================================================================================

			1) ANSI Escape Code는 Linux 터미널에서는 잘 동작하지만,
			   Windows 콘솔에서는 그대로 문자열처럼 보일 수 있다.

			2) Windows 콘솔 API 방식은 Windows 전용이다.

			3) 따라서 실제 프로그램에서는
			   플랫폼별 분기 처리를 하거나,
			   콘솔 색상 라이브러리를 쓰는 경우도 많다.


			=======================================================================================
			6. 핵심 요약
			=======================================================================================

				- 콘솔 프로그램은 텍스트 기반 프로그램이다.
				- Linux 계열은 ANSI Escape Code 로 색상 제어 가능
				- Windows 전통 콘솔은 SetConsoleTextAttribute 사용
				- 플랫폼마다 콘솔 색상 처리 방식이 다르다.
		*/


		//=========================================================================================
		// [테스트 예제 1] ANSI Escape Code - Modifier 클래스 사용
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 1] ANSI Escape Code - Modifier 클래스 사용" << std::endl;
			std::cout << "==================================================" << std::endl;

			Modifier red(FG_RED);
			Modifier def(FG_DEFAULT);

			std::cout << "This ->" << red << "word" << def << "<- is red" << std::endl;
			std::cout << std::endl;

			/*
				Linux / ANSI 지원 터미널에서는
				word 부분이 빨간색으로 출력될 수 있다.

				ANSI 미지원 콘솔에서는
				escape code 문자열이 그대로 보일 수 있다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 2] ANSI Escape Code - 매크로 직접 사용
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 2] ANSI Escape Code - 매크로 직접 사용" << std::endl;
			std::cout << "==================================================" << std::endl;

			std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

			std::cout << CLEAR;
			std::cout << MAGENTA << "Colored " << CYAN << "Text" << RESET << std::endl;

			for (size_t i = 0; i < letters.length(); ++i)
			{
				if (i % 2 == 0)
					std::cout << RED << letters[i] << RESET;
				else
					std::cout << WHITE << letters[i] << RESET;
			}

			std::cout << std::endl << std::endl;

			/*
				Linux / ANSI 지원 터미널에서는
				글자들이 빨강/흰색으로 번갈아 출력될 수 있다.
			*/
		}


		//=========================================================================================
		// [테스트 예제 3] Win32 콘솔 색상 변경
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 3] Win32 콘솔 색상 변경" << std::endl;
			std::cout << "==================================================" << std::endl;

#ifdef _WIN32
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			// 기본색(회색 계열) 비슷하게 복원
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			std::cout << "■ DEFAULT" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			std::cout << "■ FOREGROUND_RED" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
			std::cout << "■ FOREGROUND_BLUE" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
			std::cout << "■ FOREGROUND_GREEN" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE);
			std::cout << "■ FOREGROUND_RED + FOREGROUND_BLUE" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
			std::cout << "■ FOREGROUND_BLUE + FOREGROUND_GREEN" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED);
			std::cout << "■ FOREGROUND_GREEN + FOREGROUND_RED" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
			std::cout << "■ FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			std::cout << "■ FOREGROUND_INTENSITY" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "■ FOREGROUND_RED + FOREGROUND_INTENSITY" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "■ FOREGROUND_BLUE + FOREGROUND_INTENSITY" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "■ FOREGROUND_GREEN + FOREGROUND_INTENSITY" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "■ FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_INTENSITY" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "■ FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_INTENSITY" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "■ FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY" << std::endl;

			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << "■ FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_INTENSITY" << std::endl;

			// 마지막에는 기본색으로 복원
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
			std::cout << "이 테스트는 Win32 콘솔 전용입니다." << std::endl;
#endif

			std::cout << std::endl;
		}


		//=========================================================================================
		// [테스트 예제 4] 플랫폼 차이 설명
		//=========================================================================================
		{
			std::cout << "==================================================" << std::endl;
			std::cout << "[테스트 4] 플랫폼 차이 설명" << std::endl;
			std::cout << "==================================================" << std::endl;

#ifdef _WIN32
			std::cout << "현재는 Windows 계열로 보입니다." << std::endl;
			std::cout << "전통적으로 Win32 콘솔 API(SetConsoleTextAttribute)를 자주 사용합니다." << std::endl;
#else
			std::cout << "현재는 Linux/Unix 계열로 보입니다." << std::endl;
			std::cout << "ANSI Escape Code 기반 콘솔 제어가 일반적입니다." << std::endl;
#endif

			std::cout << std::endl;
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void Test()
	{
		console_programs();
	}
}


