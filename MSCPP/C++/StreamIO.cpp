#include "stdafx.h"


namespace StreamIO
{
	void basic_input_output()
	{
		/*
			📚 기본 입출력 (Basic Input/Output)

			앞의 예제들은 대부분 화면에 값만 출력했지만,
			표준 라이브러리는 사용자와 상호작용할 수 있는 다양한 입력/출력 기능을 제공한다.

			C++에서는 화면, 키보드, 파일 같은 순차적 매체와 데이터를 주고받기 위해
			스트림(stream)이라는 개념을 사용한다.

			스트림은
			- 프로그램이 문자를 넣을 수도 있고(insert)
			- 프로그램이 문자를 꺼낼 수도 있는(extract)
			하나의 흐름이라고 생각하면 된다.

			중요한 표준 스트림 객체:
				cin   : 표준 입력 스트림 (보통 키보드)
				cout  : 표준 출력 스트림 (보통 화면)
				cerr  : 표준 에러 출력 스트림
				clog  : 표준 로그 출력 스트림

			입문 단계에서는 주로 cout 과 cin 을 가장 많이 사용한다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] 기본 출력 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Hello World!" << std::endl;
			std::cout << "C++ basic input/output example" << std::endl;
			std::cout << std::endl;

			/*
				설명:
				cout 은 표준 출력 스트림이다.
				보통 콘솔 화면에 텍스트를 출력한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 기본 입력 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int number = 0;

			std::cout << "정수를 하나 입력하세요: ";
			std::cin >> number;

			std::cout << "입력한 값은 " << number << " 입니다." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				cin 은 표준 입력 스트림이다.
				보통 키보드 입력을 받는다.
			*/
		}

		system("pause");
	}

	void standard_output()
	{
		/*
			📚 표준 출력 (Standard output : cout)

			대부분의 프로그램 실행 환경에서
			표준 출력은 기본적으로 화면(console)이다.

			C++에서 표준 출력에 접근하는 스트림 객체는 cout 이다.

			cout 은 삽입 연산자 << 와 함께 사용한다.

				cout << "문자열";
				cout << 120;
				cout << x;

			의미:
			<< 뒤에 오는 값을
			<< 앞에 있는 스트림(cout)으로 보낸다.

			즉:
				cout << "Hello";
			는 "Hello"를 화면에 출력한다.

			중요:
			문자열 리터럴은 반드시 큰따옴표(")로 묶어야 한다.

				cout << "Hello";   // 문자열 Hello 출력
				cout << Hello;     // 변수 Hello를 찾으려고 함
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] 문자열 출력" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "Output sentence" << std::endl;
			std::cout << "Hello World!" << std::endl;
			std::cout << std::endl;
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 숫자 출력" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << 120 << std::endl;

			int x = 50;
			std::cout << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				cout 은 문자열뿐 아니라 숫자, 변수 값도 출력할 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 여러 << 연산을 연결해서 사용" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "This " << "is " << "a " << "single C++ statement." << std::endl;

			int age = 24;
			int zipcode = 90064;

			std::cout << "I am " << age << " years old and my zipcode is " << zipcode << "." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				<< 연산자는 여러 번 연결(chaining)해서 사용할 수 있다.
				이 방식은 문자열과 변수 값을 섞어 출력할 때 매우 편하다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 줄바꿈 없는 출력" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "This is a sentence.";
			std::cout << "This is another sentence.";
			std::cout << std::endl << std::endl;

			/*
				설명:
				cout 은 자동으로 줄바꿈을 넣지 않는다.
				따라서 두 문장이 같은 줄에 이어서 출력된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] \\n 사용한 줄바꿈" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "First sentence.\n";
			std::cout << "Second sentence.\nThird sentence.\n";
			std::cout << std::endl;

			/*
				설명:
				\n 은 줄바꿈 문자(new-line character)이다.
				원하는 위치에서 줄을 바꿀 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 6] endl 사용한 줄바꿈" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "First sentence." << std::endl;
			std::cout << "Second sentence." << std::endl;
			std::cout << std::endl;

			/*
				설명:
				endl 도 줄바꿈을 수행한다.
				추가로 출력 버퍼를 비우는(flush) 동작도 한다.

				간단한 줄바꿈만 필요하면 \n 을 자주 쓰고,
				즉시 출력 반영까지 의도할 때 endl 이 유용하다.
			*/
		}

		system("pause");
	}

	void standard_input()
	{
		/*
			📚 표준 입력 (Standard input : cin)

			대부분의 프로그램 실행 환경에서
			표준 입력은 기본적으로 키보드이다.

			C++에서 표준 입력에 접근하는 스트림 객체는 cin 이다.

			cin 은 추출 연산자 >> 와 함께 사용한다.

				int age;
				cin >> age;

			의미:
			사용자가 입력한 값을 읽어서
			변수 age 에 저장한다.

			프로그램은 cin >> 변수; 문장을 만나면
			사용자가 값을 입력하고 Enter를 누를 때까지 기다린다.

			중요:
			cin 은 변수의 타입에 맞게 입력을 해석한다.
			예를 들어 int 변수면 정수 형태를 기대한다.

			주의:
			잘못된 형식의 값이 들어오면 입력 실패가 발생할 수 있다.
			그래서 실무에서는 cin만 바로 믿고 쓰기보다는
			getline + stringstream 방식이 더 안전한 경우가 많다.
		*/

		{
			int i;

			std::cout << "Please enter an integer value: ";
			std::cin >> i;

			std::cout << "The value you entered is " << i;
			std::cout << " and its double is " << i * 2 << ".\n";
			std::cout << std::endl;

			/*
				예시 입력:
					702

				예시 출력:
					The value you entered is 702 and its double is 1404.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 여러 값 한 번에 입력" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 0;
			int b = 0;

			std::cout << "정수 2개를 입력하세요: ";
			std::cin >> a >> b;

			std::cout << "a = " << a << ", b = " << b << std::endl;
			std::cout << "a + b = " << a + b << std::endl;
			std::cout << std::endl;

			/*
				설명:
				cin >> a >> b;
				는
				cin >> a;
				cin >> b;
				와 같은 의미이다.

				입력값은 공백, 탭, 줄바꿈으로 구분될 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] cin 사용 시 주의점 안내" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "cin >> int 변수; 형태는 간단하지만" << std::endl;
			std::cout << "사용자가 정수가 아닌 값을 입력하면 실패할 수 있다." << std::endl;
			std::cout << "실제 프로그램에서는 입력 검증이 중요하다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void cin_n_strings()
	{
		/*
			📚 cin 과 문자열 (cin and strings)

			cin >> 문자열변수;
			형태로 문자열도 입력받을 수 있다.

			예:
				std::string mystring;
				std::cin >> mystring;

			하지만 중요한 특징이 있다.

			cin >> 문자열변수; 는
			공백(space), 탭(tab), 줄바꿈(new-line)을 만나면
			입력을 끝낸다.

			즉:
			한 단어(word)는 잘 읽지만,
			문장(sentence) 전체는 읽지 못한다.

			문장 한 줄 전체를 입력받으려면
			getline 함수를 사용한다.

				std::getline(std::cin, 문자열변수);

			getline 은 Enter를 누를 때까지 한 줄 전체를 읽는다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] cin >> string 은 한 단어만 입력" << std::endl;
			std::cout << "============================================" << std::endl;

			std::string word;

			std::cout << "한 단어를 입력하세요: ";
			std::cin >> word;

			std::cout << "입력한 단어: " << word << std::endl;
			std::cout << std::endl;

			/*
				설명:
				예를 들어 "Homer Simpson" 을 입력하면
				cin >> word; 는 보통 Homer 까지만 읽는다.
			*/
		}

		// 앞선 cin >> 입력 뒤에 남아 있는 개행 문자 정리
		std::cin.ignore(10000, '\n');

		{
			std::string mystr;

			std::cout << "What's your name? ";
			std::getline(std::cin, mystr);
			std::cout << "Hello " << mystr << ".\n";

			std::cout << "What is your favorite team? ";
			std::getline(std::cin, mystr);
			std::cout << "I like " << mystr << " too!\n";

			std::cout << std::endl;

			/*
				예시 입력:
					Homer Simpson
					The Isotopes

				예시 출력:
					Hello Homer Simpson.
					I like The Isotopes too!

				설명:
				getline 은 공백 포함 한 줄 전체를 읽는다.
				따라서 이름이나 문장 입력에 적합하다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 같은 문자열 변수 재사용" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "getline 을 여러 번 호출하면" << std::endl;
			std::cout << "같은 문자열 변수에 새로운 내용이 덮어써진다." << std::endl;
			std::cout << std::endl;
		}

		/*
			실무 팁:
			콘솔 프로그램에서는
			사용자가 보통 "한 줄 입력 후 Enter" 방식에 익숙하므로

			가능하면
				getline 으로 먼저 한 줄 전체를 읽고
			그 다음 필요하면 숫자로 변환하는 방식이
			더 안정적이고 사용자 친화적이다.
		*/

		system("pause");
	}

#include <sstream>

	void stringstream_func()
	{
		/*
			📚 stringstream

			<sstream> 헤더에는 stringstream 이라는 타입이 있다.

			stringstream 은 문자열을
			cin / cout 처럼 스트림처럼 다룰 수 있게 해준다.

			즉:
			- 문자열에서 값을 추출할 수도 있고
			- 문자열에 값을 넣을 수도 있다.

			특히 많이 쓰이는 용도:
			1) 문자열 -> 숫자 변환
			2) 숫자 -> 문자열 조합
			3) 한 줄 입력을 세부 값들로 분리

			예:
				std::string mystr("1204");
				int myint;
				std::stringstream(mystr) >> myint;

			결과:
				myint = 1204
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] 문자열을 정수로 변환" << std::endl;
			std::cout << "============================================" << std::endl;

			std::string mystr("1204");
			int myint = 0;

			std::stringstream(mystr) >> myint;

			std::cout << "mystr = " << mystr << std::endl;
			std::cout << "myint = " << myint << std::endl;
			std::cout << std::endl;
		}

		{
			std::string mystr;
			float price = 0.0f;
			int quantity = 0;

			std::cout << "Enter price: ";
			std::getline(std::cin, mystr);
			std::stringstream(mystr) >> price;

			std::cout << "Enter quantity: ";
			std::getline(std::cin, mystr);
			std::stringstream(mystr) >> quantity;

			std::cout << "Total price: " << price * quantity << std::endl;
			std::cout << std::endl;

			/*
				예시 입력:
					22.25
					7

				예시 출력:
					Total price: 155.75

				설명:
				직접 cin >> price; cin >> quantity; 를 쓰지 않고,
				먼저 getline 으로 문자열 한 줄을 받은 뒤
				stringstream 으로 숫자로 변환했다.

				이 방식은
				입력 받기와 해석하기를 분리할 수 있어서 더 유연하다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 문자열을 여러 값으로 분리" << std::endl;
			std::cout << "============================================" << std::endl;

			std::string line = "100 200 300";
			int a = 0, b = 0, c = 0;

			std::stringstream ss(line);
			ss >> a >> b >> c;

			std::cout << "line = " << line << std::endl;
			std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
			std::cout << std::endl;

			/*
				설명:
				stringstream 은 문자열을 입력 스트림처럼 다룰 수 있으므로
				공백으로 구분된 값들을 손쉽게 분리할 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 값들을 문자열로 합치기" << std::endl;
			std::cout << "============================================" << std::endl;

			int id = 101;
			float score = 95.5f;

			std::stringstream ss;
			ss << "ID=" << id << ", Score=" << score;

			std::string result = ss.str();

			std::cout << "result = " << result << std::endl;
			std::cout << std::endl;

			/*
				설명:
				stringstream 은 출력 스트림처럼도 사용할 수 있다.
				여러 값을 이어 붙여 문자열을 만들 때도 유용하다.
			*/
		}

		/*
			핵심 장점:
			1) getline 으로 한 줄 전체 입력
			2) stringstream 으로 필요한 타입으로 변환

			이 방식은
			사용자 입력 처리와 데이터 변환을 분리해 주므로
			더 안정적인 콘솔 프로그램을 만들 때 유용하다.
		*/

		system("pause");
	}

	void Test()
	{
		//basic_input_output();

		//standard_output();

		//standard_input();

		//cin_n_strings();

		//stringstream_func();

		//stringstream_func();
	}

}// end of StreamIO