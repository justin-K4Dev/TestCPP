#include "stdafx.h"



namespace StructureOfProgram
{

	void structure_of_program()
	{
		/*
			📚 프로그램의 구조 (Structure of program)

			프로그래밍 언어를 배우는 가장 좋은 방법은 직접 프로그램을 작성해보는 것이다.
			보통 가장 먼저 만드는 프로그램은 "Hello World" 프로그램이며,
			화면에 단순히 Hello World를 출력한다.

			비록 매우 단순한 프로그램이지만,
			C++ 프로그램의 핵심 구성 요소를 모두 포함하고 있다.

			예시 형태:

				#include <iostream>

				int main()
				{
					std::cout << "Hello World!";
				}

			구성 요소 설명:
			1) #include <iostream>
			   표준 입출력을 사용하기 위한 헤더 포함

			2) int main()
			   프로그램이 시작되는 특별한 함수

			3) { ... }
			   함수의 시작과 끝을 나타내는 중괄호

			4) std::cout << "Hello World!";
			   실제로 화면에 문장을 출력하는 실행문(statement)

			5) ;
			   문장의 끝을 나타내는 세미콜론
		*/

		{
			std::cout << "Hello World!";

			/*
				출력 결과:
					Hello World!

				설명:
				std::cout 는 표준 출력 장치(보통 콘솔 화면)에
				문자열을 출력한다.
			*/
		}

		{
			std::cout << std::endl;
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 여러 문장을 순서대로 실행" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Hello World! ";
			std::cout << "I'm a C++ program";
			std::cout << std::endl;

			/*
				출력 결과:
					Hello World! I'm a C++ program

				설명:
				C++에서는 문장이 나타나는 순서대로 실행된다.
				여기서는 std::cout 를 두 번 사용해서
				문자열 두 개를 순서대로 이어서 출력했다.
			*/
		}

		{
			std::cout << std::endl;
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 줄바꿈은 사람이 보기 좋게 만드는 역할" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "첫 번째 문장" << std::endl;
			std::cout << "두 번째 문장" << std::endl;
			std::cout << "세 번째 문장" << std::endl;
			std::cout << std::endl;

			/*
				설명:
				C++는 줄바꿈 자체로 문장을 구분하지 않는다.
				문장의 끝은 세미콜론(;)이 결정한다.

				즉, 코드를 여러 줄로 쓰는 이유는
				사람이 읽기 쉽게 만들기 위해서이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 세미콜론(;)의 중요성" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "모든 C++ 실행문은 보통 세미콜론으로 끝난다." << std::endl;
			std::cout << "세미콜론을 빼먹으면 매우 흔한 문법 오류가 발생한다." << std::endl;
			std::cout << std::endl;

			/*
				예:
					std::cout << "Hello";   // 정상
					std::cout << "Hello"    // 오류 가능

				설명:
				세미콜론은 문장의 끝을 표시한다.
			*/
		}

		system("pause");
	}

	void comments()
	{
		/*
			📚 주석 (Comments)

			주석은 프로그램 실행에는 영향을 주지 않지만,
			소스 코드 안에 설명을 남길 수 있는 매우 중요한 도구이다.

			C++에서는 두 가지 방식의 주석을 지원한다.

			1) 한 줄 주석
				// 내용

			   // 가 나온 위치부터 그 줄 끝까지 모두 주석 처리된다.

			2) 여러 줄 주석(블록 주석)
				/* 내용 *\/

			   시작과 끝 사이의 모든 내용을 주석 처리한다.
			   여러 줄에 걸쳐 작성할 수 있다.
		*/

		// 한 줄 주석 예제
		/* 여러 줄 주석 예제 */

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] 한 줄 주석과 여러 줄 주석" << std::endl;
			std::cout << "============================================" << std::endl;

			/* 이 프로그램은 Hello World를 출력하는 예제이다. */

			std::cout << "Hello World! ";     // Hello World 출력
			std::cout << "I'm a C++ program"; // 두 번째 문장 출력
			std::cout << std::endl;

			/*
				출력 결과:
					Hello World! I'm a C++ program

				설명:
				주석은 사람이 코드를 이해하기 쉽게 설명을 붙이는 용도이며,
				컴파일러는 주석 내용을 실행하지 않는다.
			*/
		}

		{
			std::cout << std::endl;
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 주석은 실행에 영향을 주지 않음" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 10;
			// a = 999;   // 이 코드는 주석 처리되었으므로 실행되지 않음
			std::cout << "a = " << a << std::endl;
			std::cout << std::endl;

			/*
				설명:
				주석 처리된 코드는
				실제로 없는 것처럼 취급된다.

				따라서 a는 여전히 10이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 여러 줄 설명에 블록 주석 사용" << std::endl;
			std::cout << "============================================" << std::endl;

			/*
				여러 줄에 걸쳐
				설명을 작성하고 싶을 때는
				블록 주석이 편리하다.
			*/

			std::cout << "블록 주석 예제" << std::endl;
			std::cout << std::endl;
		}

		/*
			주의:
			주석 기호 없이 일반 문장을 코드 중간에 써버리면
			컴파일러는 그것을 C++ 코드로 해석하려고 하므로
			컴파일 오류가 발생할 가능성이 매우 높다.
		*/

		system("pause");
	}

	void using_namespace_std()
	{
		/*
			📚 using namespace std

			C++ 표준 라이브러리의 많은 요소들은 std 라는 네임스페이스(namespace) 안에 들어 있다.

			예를 들어:
				std::cout
				std::cin
				std::string
				std::endl

			여기서 std:: 는
			"std 네임스페이스 안에 있는 요소"라는 뜻이다.

			따라서 cout 을 정확하게 쓰려면:

				std::cout

			처럼 작성해야 한다.

			하지만 아래와 같이 쓰면:

				using namespace std;

			std 네임스페이스 안의 요소들을
			std:: 없이 바로 사용할 수 있다.

				cout
				string
				endl

			즉:
				std::cout << "Hello";
			와
				cout << "Hello";
			는 같은 대상을 가리킨다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] std:: 를 직접 붙여서 사용" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Hello World! ";
			std::cout << "I'm a C++ program";
			std::cout << std::endl;

			/*
				설명:
				std::cout 처럼 직접 std:: 를 붙이면
				어느 네임스페이스의 cout 인지 명확하다.
			*/
		}

		{
			std::cout << std::endl;
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] using namespace std 사용" << std::endl;
			std::cout << "============================================" << std::endl;

			using namespace std;

			cout << "Hello World! ";
			cout << "I'm a C++ program";
			cout << endl;

			/*
				출력 결과:
					Hello World! I'm a C++ program

				설명:
				using namespace std; 를 선언하면
				std 안에 있는 이름들을 std:: 없이 사용할 수 있다.
			*/
		}

		{
			std::cout << std::endl;
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] string 도 std 네임스페이스에 속함" << std::endl;
			std::cout << "============================================" << std::endl;

			using namespace std;

			string text = "Namespace test";
			cout << "text = " << text << endl;
			cout << endl;

			/*
				설명:
				string 역시 std 네임스페이스 안에 있기 때문에
				using namespace std; 가 있으면
				std::string 대신 string 만 써도 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 어떤 방식을 쓰는 것이 좋은가?" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "1) std::cout 처럼 직접 쓰는 방식" << std::endl;
			std::cout << "2) using namespace std; 후 cout 만 쓰는 방식" << std::endl;
			std::cout << std::endl;
			std::cout << "둘 다 가능하지만, std:: 를 직접 쓰면 이름 충돌을 줄이기 쉽다." << std::endl;
			std::cout << "학습용 예제에서는 using namespace std; 가 더 간단해 보일 수 있다." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				실무에서는 전역 범위에서 using namespace std; 를
				무분별하게 사용하는 것을 피하는 경우가 많다.

				이유:
				이름 충돌(name collision) 가능성이 있기 때문이다.

				하지만 학습 단계에서는
				코드를 간단하게 보기 위해 자주 사용한다.
			*/
		}

		system("pause");
	}

	void Test()
	{
		//structure_of_program();

		//comments();

		//using_namespace_std();
	}

}// end of StructureOfProgram
