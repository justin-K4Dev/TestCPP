#include "stdafx.h"



namespace CharacterSequences
{
	void character_sequence()
	{
		/*
			📚 문자 시퀀스 (Character sequence)

			문자열(string)은 결국 문자(char)들이 순서대로 나열된 것이다.

			C++에서는 문자열을 다루는 편리한 std::string 클래스가 있지만,
			문자 배열(char 배열)로도 문자열을 표현할 수 있다.

			예:
				char foo[20];

			이 배열은 char 20개를 저장할 수 있는 배열이다.
			즉 최대 20개의 문자를 담을 수 있는 공간이라고 볼 수 있다.

			하지만 실제 문자열을 저장할 때는
			문자들만 들어가는 것이 아니라
			문자열의 끝을 나타내는 특별한 문자도 함께 필요하다.

			그 특별한 문자가 바로:
				'\0'
			이다.

			이 문자를 null 문자(null terminator)라고 한다.

			C 스타일 문자열(C-string)은
			문자들이 나열된 뒤 마지막에 '\0' 이 붙어 있는 char 배열이다.
		*/

		{
			char foo[20] = "Hello";

			std::cout << "foo = " << foo << std::endl;
			std::cout << "문자열 길이(눈에 보이는 글자 수)는 5이지만," << std::endl;
			std::cout << "실제로는 마지막에 '\\0' 이 포함되어 저장된다." << std::endl;
			std::cout << std::endl;

			/*
				내부 개념:
					[H][e][l][l][o]['\0'][...]
			*/
		}

		{
			char foo[20] = "Merry Christmas";

			std::cout << "foo = " << foo << std::endl;
			std::cout << std::endl;

			/*
				설명:
				foo 는 길이 20짜리 배열이므로
				"Merry Christmas" 같은 더 긴 문자열도 저장 가능하다.

				하지만 끝에는 반드시 '\0' 이 들어가야
				문자열의 끝을 알 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] null 문자 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			char word[] = "Hello";

			for (int i = 0; i < 6; ++i)
			{
				if (word[i] == '\0')
					std::cout << "word[" << i << "] = '\\0'" << std::endl;
				else
					std::cout << "word[" << i << "] = " << word[i] << std::endl;
			}
			std::cout << std::endl;

			/*
				출력 개념:
					word[0] = H
					word[1] = e
					word[2] = l
					word[3] = l
					word[4] = o
					word[5] = '\0'

				즉 문자열 끝에는 null 문자가 존재한다.
			*/
		}

		system("pause");
	}

	void initialization_of_null_terminated_character_sequences()
	{
		/*
			📚 null 종료 문자 배열 초기화
			(Initialization of null-terminated character sequences)

			C 스타일 문자열은 결국 char 배열이므로
			배열 초기화 규칙을 그대로 따른다.

			즉 문자열 "Hello" 는 다음처럼 직접 문자 하나씩 넣어도 되고:

				char myWord[] = { 'H', 'e', 'l', 'l', 'o', '\0' };

			문자열 리터럴을 사용해 더 간단히 쓸 수도 있다:

				char myWord[] = "Hello";

			중요:
			두 번째 방식에서는 마지막 '\0' 이 자동으로 붙는다.
		*/

		{
			char myWord1[] = { 'H', 'e', 'l', 'l', 'o', '\0' };
			char myWord2[] = "Hello";

			std::cout << "myWord1 = " << myWord1 << std::endl;
			std::cout << "myWord2 = " << myWord2 << std::endl;
			std::cout << std::endl;

			/*
				설명:
				두 배열 모두 결과적으로 같은 문자열을 표현한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 배열 크기 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			char myWord[] = "Hello";

			std::cout << "sizeof(myWord) = " << sizeof(myWord) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				"Hello" 는 글자 5개이지만
				끝에 '\0' 이 자동으로 추가되므로
				배열 크기는 6이 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 선언 후 전체 대입은 불가능" << std::endl;
			std::cout << "============================================" << std::endl;

			char myWord[10] = "Hello";

			// myWord = "Bye";   // 오류
			// myWord[] = "Bye"; // 오류

			myWord[0] = 'B';
			myWord[1] = 'y';
			myWord[2] = 'e';
			myWord[3] = '\0';

			std::cout << "myWord = " << myWord << std::endl;
			std::cout << std::endl;

			/*
				설명:
				배열은 선언 후에
				문자열 전체를 통째로 = 로 다시 대입할 수 없다.

				대신 각 요소를 직접 바꿔야 한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] null 문자 누락 시 문제" << std::endl;
			std::cout << "============================================" << std::endl;

			char badWord[5] = { 'H', 'e', 'l', 'l', 'o' };

			std::cout << "badWord 는 '\\0' 이 없어서 C 문자열로 안전하지 않다." << std::endl;
			std::cout << "즉 cout << badWord; 같은 사용은 위험할 수 있다." << std::endl;
			std::cout << std::endl;

			/*
				중요:
				C 문자열은 반드시 끝에 '\0' 이 있어야 한다.
			*/
		}

		system("pause");
	}


	void strings_n_null_terminated_character_sequences()
	{
		/*
			📚 문자열(string)과 null 종료 문자 시퀀스
			(Strings and null-terminated character sequences)

			C++에는 문자열을 표현하는 방법이 크게 두 가지 있다.

			1) C 스타일 문자열
				char 배열 + 마지막 '\0'

			2) C++ 표준 문자열
				std::string

			두 방식은 함께 많이 쓰이며,
			표준 라이브러리의 많은 기능이 둘 다 지원한다.

			예를 들어 cout 은
			C 문자열도 출력할 수 있고 std::string 도 출력할 수 있다.
		*/

		{
			char question1[] = "What is your name? ";
			std::string question2 = "Where do you live? ";

			char answer1[80];
			std::string answer2;

			std::cout << question1;
			std::cin >> answer1;

			std::cout << question2;
			std::cin >> answer2;

			std::cout << "Hello, " << answer1;
			std::cout << " from " << answer2 << "!\n";
			std::cout << std::endl;

			/*
				예시 입력:
					Homer
					Greece

				예시 출력:
					Hello, Homer from Greece!

				설명:
				answer1 은 C 스타일 문자열(char 배열)
				answer2 는 std::string 이다.

				둘 다 cin / cout 과 함께 사용할 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] C 문자열 -> std::string 변환" << std::endl;
			std::cout << "============================================" << std::endl;

			char myntcs[] = "some text";
			std::string mystring = myntcs;

			std::cout << "myntcs   = " << myntcs << std::endl;
			std::cout << "mystring = " << mystring << std::endl;
			std::cout << std::endl;

			/*
				설명:
				C 문자열은 std::string 으로 쉽게 변환할 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] std::string -> C 문자열 변환" << std::endl;
			std::cout << "============================================" << std::endl;

			std::string mystring = "Hello Justin";

			std::cout << "mystring       = " << mystring << std::endl;
			std::cout << "mystring.c_str = " << mystring.c_str() << std::endl;
			std::cout << "mystring.data  = " << mystring.data() << std::endl;
			std::cout << std::endl;

			/*
				설명:
				std::string 을 C 스타일 문자열처럼 사용하려면
				c_str() 또는 data() 를 사용할 수 있다.

				즉:
					mystring.c_str()
				는 내부 문자열을 null-terminated char 시퀀스로 제공한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 크기 차이" << std::endl;
			std::cout << "============================================" << std::endl;

			char cstr[10] = "Hello";
			std::string str = "Hello";

			std::cout << "sizeof(cstr) = " << sizeof(cstr) << std::endl;
			std::cout << "str.size()   = " << str.size() << std::endl;
			std::cout << std::endl;

			/*
				설명:
				C 문자열 배열은 선언 시 크기가 고정된다.
				즉 cstr 은 길이 10짜리 공간이다.

				반면 std::string 은
				실행 중 길이가 동적으로 관리된다.

				str.size() 는 실제 글자 수인 5를 반환한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] cin >> char배열 과 cin >> string 의 공통점" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "둘 다 공백 전까지만 입력받는다." << std::endl;
			std::cout << "문장 전체를 받고 싶으면 getline 을 써야 한다." << std::endl;
			std::cout << std::endl;

			/*
				예:
					char name[80];
					std::string text;

					std::cin >> name;
					std::cin >> text;

				둘 다 공백에서 끊긴다.
			*/
		}

		system("pause");
	}

	void Test()
	{
		//character_sequence();

		//initialization_of_null_terminated_character_sequences();

		//strings_n_null_terminated_character_sequences();
	}

}// end of CharacterSequences