#include "stdafx.h"


namespace Arrays
{
	void printArray(int arg[], int length)
	{
		for (int n = 0; n<length; ++n)
			std::cout << arg[n] << ' ';
		std::cout << '\n';
	}

	void arrays()
	{
		/*
			📚 배열 (Arrays)

			배열은 같은 자료형의 여러 값을
			메모리에 연속적으로 저장하는 자료구조이다.

			예:
				int foo[5];

			이 뜻은:
				int형 값 5개를 연속된 메모리 공간에 만든다.

			배열의 각 요소는 인덱스(index)로 접근한다.
			인덱스는 항상 0부터 시작한다.

				foo[0]  -> 첫 번째 요소
				foo[1]  -> 두 번째 요소
				foo[4]  -> 다섯 번째 요소

			중요:
			C++ 배열 크기는 기본적으로 컴파일 시점에 결정되어야 한다.
			즉 [] 안의 크기는 보통 상수여야 한다.
		*/

		{
			int foo[5]; // int 5개짜리 배열 선언

			foo[0] = 10;
			foo[1] = 20;
			foo[2] = 30;
			foo[3] = 40;
			foo[4] = 50;

			std::cout << "foo[0] = " << foo[0] << std::endl;
			std::cout << "foo[1] = " << foo[1] << std::endl;
			std::cout << "foo[2] = " << foo[2] << std::endl;
			std::cout << "foo[3] = " << foo[3] << std::endl;
			std::cout << "foo[4] = " << foo[4] << std::endl;
			std::cout << std::endl;

			/*
				설명:
				배열 이름은 같지만,
				인덱스로 각 요소를 구분해서 접근한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 배열은 같은 타입 여러 개를 묶는다" << std::endl;
			std::cout << "============================================" << std::endl;

			int scores[3];
			scores[0] = 100;
			scores[1] = 95;
			scores[2] = 88;

			for (int i = 0; i < 3; ++i)
			{
				std::cout << "scores[" << i << "] = " << scores[i] << std::endl;
			}
			std::cout << std::endl;
		}

		/*
			주의:
				foo[5]
			는 6번째 요소 접근이다.

			배열 크기가 5라면 유효 인덱스는 0~4 까지이다.
			범위를 벗어나면 컴파일 오류가 아니라 런타임 문제를 일으킬 수 있다.
		*/

		system("pause");
	}


	void initializing_arrays()
	{
		/*
			📚 배열 초기화 (Initializing arrays)

			지역 배열은 선언만 하면 기본적으로 값이 정해져 있다고 보장되지 않는다.
			따라서 필요하면 선언과 동시에 초기화하는 것이 중요하다.

			대표적인 초기화 방식:

				int a[5] = { 1, 2, 3, 4, 5 };
				int b[5] = { 10, 20, 30 };
				int c[5] = { };
				int d[]  = { 7, 8, 9 };

			규칙:
			1) 초기값 개수가 배열 크기보다 적으면 나머지는 0으로 채워진다.
			2) 중괄호를 비우면 전체가 0으로 초기화된다.
			3) 크기를 생략하면 초기값 개수만큼 배열 크기가 결정된다.
		*/

		{
			int a[5] = { 16, 2, 77, 40, 12071 };

			printArray(a, 5);
			std::cout << std::endl;

			/*
				결과:
					16 2 77 40 12071
			*/
		}

		{
			int b[5] = { 10, 20, 30 };

			printArray(b, 5);
			std::cout << std::endl;

			/*
				결과:
					10 20 30 0 0

				설명:
				초기값이 3개뿐이므로 나머지 2개는 0으로 채워진다.
			*/
		}

		{
			int c[5] = { };

			printArray(c, 5);
			std::cout << std::endl;

			/*
				결과:
					0 0 0 0 0
			*/
		}

		{
			int d[] = { 1, 2, 3, 4 };

			printArray(d, 4);
			std::cout << std::endl;

			/*
				설명:
				배열 크기를 생략하면
				초기값 개수만큼 자동으로 크기가 정해진다.
			*/
		}

		{
			int value1[5] = {};
			int value2[5] = { 0 };
			int value3[5] = { 1 };

			printArray(value1, 5); // 0 0 0 0 0
			printArray(value2, 5); // 0 0 0 0 0
			printArray(value3, 5); // 1 0 0 0 0
			std::cout << std::endl;
		}

		{
			int data[5] = { 1 };
			std::fill_n(data, 5, 1);

			printArray(data, 5);
			std::cout << std::endl;

			/*
				설명:
				{1} 은 첫 번째 요소만 1이고 나머지는 0이다.
				배열 전체를 같은 값으로 채우려면 std::fill_n 이 편리하다.
			*/
		}

		{
			char str1[5] = {};
			char str2[5] = { '\0' };

			std::cout << "str1[0] = " << (int)str1[0] << std::endl;
			std::cout << "str2[0] = " << (int)str2[0] << std::endl;
			std::cout << std::endl;

			/*
				설명:
				char 배열도 {} 또는 {'\0'} 로 전체를 0 문자로 초기화할 수 있다.
			*/
		}

		system("pause");
	}


	void accessing_the_values_of_an_array()
	{
		/*
			📚 배열 요소 접근 (Accessing the values of an array)

			배열의 각 요소는
				배열이름[인덱스]
			형태로 접근한다.

			예:
				foo[2] = 75;
				x = foo[2];

			중요:
			배열 이름 뒤의 [] 는
			1) 선언 시 크기 지정
			2) 사용 시 특정 요소 접근
			두 가지 용도로 쓰인다.

				int foo[5];   // 선언
				foo[2] = 75;  // 접근
		*/

		{
			int foo[] = { 16, 2, 77, 40, 12071 };

			foo[2] = 75;

			std::cout << "foo[0] = " << foo[0] << std::endl;
			std::cout << "foo[1] = " << foo[1] << std::endl;
			std::cout << "foo[2] = " << foo[2] << std::endl;
			std::cout << "foo[3] = " << foo[3] << std::endl;
			std::cout << "foo[4] = " << foo[4] << std::endl;
			std::cout << std::endl;
		}

		{
			int foo[] = { 16, 2, 77, 40, 12071 };
			int n, result = 0;

			for (n = 0; n < 5; ++n)
			{
				result += foo[n];
			}

			std::cout << result << std::endl;
			std::cout << std::endl;

			/*
				출력:
					12206

				설명:
				반복문으로 배열 전체를 순회하면서 합계를 구했다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 인덱스도 식(expression)이 될 수 있다" << std::endl;
			std::cout << "============================================" << std::endl;

			int foo[] = { 10, 20, 30, 40, 50 };
			int a = 1;

			std::cout << "foo[a]     = " << foo[a] << std::endl;
			std::cout << "foo[a + 2] = " << foo[a + 2] << std::endl;
			std::cout << std::endl;

			/*
				설명:
				인덱스는 숫자 리터럴만 아니라 식도 가능하다.
			*/
		}

		/*
			중요 주의:
			C++은 배열 범위를 자동 검사하지 않는다.

				foo[5]

			같은 잘못된 접근은 컴파일 오류가 아닐 수 있으며,
			런타임 버그나 메모리 오염을 일으킬 수 있다.
		*/

		system("pause");
	}


	void multidimensional_arrays()
	{
		/*
			📚 다차원 배열 (Multidimensional arrays)

			다차원 배열은 "배열의 배열"이라고 생각하면 된다.

			예:
				int table[3][5];

			이 의미는:
				3행 5열의 int 배열

			접근:
				table[행][열]

			예:
				table[1][3]
				-> 두 번째 행, 네 번째 열
		*/

		{
			int table[3][5] = { 0 };

			for (int row = 0; row < 3; ++row)
			{
				for (int col = 0; col < 5; ++col)
				{
					table[row][col] = (row + 1) * (col + 1);
				}
			}

			for (int row = 0; row < 3; ++row)
			{
				for (int col = 0; col < 5; ++col)
				{
					std::cout << table[row][col] << ' ';
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;

			/*
				출력:
					1 2 3 4 5
					2 4 6 8 10
					3 6 9 12 15
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 2차원 배열도 결국 연속 메모리" << std::endl;
			std::cout << "============================================" << std::endl;

			int arr[2][3] = { {1,2,3}, {4,5,6} };

			std::cout << "arr[0][0] = " << arr[0][0] << std::endl;
			std::cout << "arr[0][1] = " << arr[0][1] << std::endl;
			std::cout << "arr[1][2] = " << arr[1][2] << std::endl;
			std::cout << std::endl;
		}

		/*
			실무 팁:
			다차원 배열은 편리하지만,
			차원이 늘수록 메모리 크기가 급격히 커진다.

			예:
				char century[100][365][24][60][60];

			이런 배열은 엄청난 메모리를 사용한다.
		*/

		system("pause");
	}

	void arrays_of_strings()
	{
		/*
			📚 문자열 배열 (Arrays of strings)

			C 스타일 문자열은
			문자(char) 배열의 끝에 '\0' 문자가 붙은 형태이다.

			예:
				char greeting[] = "Hello";

			실제 내부:
				'H' 'e' 'l' 'l' 'o' '\0'

			즉 문자열은 사실상 char 배열이다.

			문자열 여러 개를 저장하는 방법은 크게 2가지로 자주 본다.

			1) 2차원 char 배열
			2) char* 배열 (문자열 리터럴 포인터 목록)
		*/

		{
			char greeting[6] = { 'H', 'e', 'l', 'l', 'o', '\0' };
			std::cout << greeting << std::endl;

			/*
				출력:
					Hello
			*/
		}

		{
			char greeting[] = "Hello";
			std::cout << greeting << std::endl;
			std::cout << std::endl;

			/*
				설명:
				문자열 리터럴로 초기화하면
				끝의 '\0' 까지 포함해서 배열 크기가 정해진다.
			*/
		}

		{
			char names[3][10] =
			{
				"Justin",
				"Hello",
				"Good"
			};

			for (int i = 0; i < 3; ++i)
			{
				std::cout << names[i] << std::endl;
			}
			std::cout << std::endl;

			/*
				설명:
				2차원 char 배열은 "문자열 배열"처럼 사용할 수 있다.
				각 행이 하나의 문자열이다.
			*/
		}

		{
			const char* greetingList[] = { "Hello", "Welcome", "to justin World !!!" };

			for (int i = 0; i < 3; ++i)
			{
				std::cout << greetingList[i] << std::endl;
			}
			std::cout << std::endl;

			/*
				설명:
				이 방식은 문자열 리터럴들의 주소를 모아 놓은 배열이다.
				2차원 char 배열과는 구조가 다르다.
			*/
		}

		system("pause");
	}


	void const_size_array_params(int params1[1], int params2[1][2], int params3[1][2][3])
	{
		params1[0] = 1;

		params2[0][0] = 1;
		params2[0][1] = 2;

		params3[0][0][0] = 1;
		params3[0][1][1] = 2;
		params3[0][1][2] = 3;
	}

	void pointer_size_array_params(int* params1, int (*params2)[2], int (*params3)[2][3], int count)
	{
		for (int i = 0; i < (count - 2); ++i)
		{
			params1[i] = i + 1;
		}

		for (int i = 0; i < (count - 2); ++i)
		{
			for (int j = 0; j < (count - 1); ++j)
			{
				params2[i][j] = j + 1;
			}
		}

		for (int i = 0; i < (count - 2); ++i)
		{
			for (int j = 0; j < (count - 1); ++j)
			{
				for (int k = 0; k < count; ++k)
				{
					params3[i][j][k] = k + 1;
				}
			}
		}
	}

	template<int size1>
	void template_type_array_params(int params[size1])
	{
		for (int i = 0; i < size1; ++i)
		{
			params[i] = i + 1;
		}
	}

	void arrays_as_parameters()
	{
		/*
			📚 배열을 함수 인자로 넘기기 (Arrays as parameters)

			C++에서는 배열 전체가 함수로 "복사되어" 전달되는 것이 아니라,
			대부분 배열의 주소가 전달된다.

			즉:
				void procedure(int arg[])

			이 형태는 사실상
				void procedure(int* arg)

			와 비슷하게 동작한다.

			중요:
			배열 길이 정보는 자동으로 같이 전달되지 않는다.
			그래서 보통 길이를 따로 인자로 넘겨야 한다.
		*/

		{
			int firstArray[] = { 5, 10, 15 };
			int secondArray[] = { 2, 4, 6, 8, 10 };

			printArray(firstArray, 3);
			printArray(secondArray, 5);
			std::cout << std::endl;

			/*
				출력:
					5 10 15
					2 4 6 8 10
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 1차원 배열 인자" << std::endl;
			std::cout << "============================================" << std::endl;

			int params1[1];
			template_type_array_params<1>(params1);

			std::cout << "params1[0] = " << params1[0] << std::endl;
			std::cout << std::endl;
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 고정 크기 다차원 배열 인자" << std::endl;
			std::cout << "============================================" << std::endl;

			int params1[1] = { 0 };
			int params2[1][2] = { 0 };
			int params3[1][2][3] = { 0 };

			const_size_array_params(params1, params2, params3);

			std::cout << "params1[0] = " << params1[0] << std::endl;
			std::cout << "params2[0][0] = " << params2[0][0] << std::endl;
			std::cout << "params2[0][1] = " << params2[0][1] << std::endl;
			std::cout << "params3[0][0][0] = " << params3[0][0][0] << std::endl;
			std::cout << "params3[0][1][1] = " << params3[0][1][1] << std::endl;
			std::cout << "params3[0][1][2] = " << params3[0][1][2] << std::endl;
			std::cout << std::endl;
		}

		/*
			중요 포인트:
			다차원 배열을 함수 인자로 받을 때는
			첫 번째 차원만 생략 가능하고,
			나머지 차원 크기는 컴파일러가 알아야 한다.

			예:
				void func(int arr[][3][4])

			이유:
			컴파일러가 다음 원소 위치 계산을 해야 하기 때문이다.
		*/

		system("pause");
	}


#include <array>

	void library_arrays()
	{
		/*
			📚 라이브러리 배열 (std::array)

			C 스타일 기본 배열은 빠르고 단순하지만,
			다음과 같은 아쉬움이 있다.

			- 복사가 불편하거나 제한적
			- 쉽게 포인터로 decay 됨
			- size 정보를 함수 안에서 잃기 쉬움

			이를 보완하기 위해 C++ 표준 라이브러리는
			<std::array> 를 제공한다.

			형태:
				std::array<int, 3> arr = { 10, 20, 30 };

			장점:
			1) 크기를 객체가 알고 있다
			2) 복사 가능
			3) 포인터로 자동 붕괴되지 않음
			4) 범위 기반 for 문과 잘 어울린다
		*/

		{
			int myarray1[3] = { 10, 20, 30 };

			for (int i = 0; i < 3; ++i)
				++myarray1[i];

			for (int i = 0; i < 3; ++i)
				std::cout << myarray1[i] << std::endl;

			std::cout << std::endl;
		}

		{
			std::array<int, 3> myarray2 = { 10, 20, 30 };

			for (int i = 0; i < (int)myarray2.size(); ++i)
				++myarray2[i];

			for (int elem : myarray2)
				std::cout << elem << std::endl;

			std::cout << std::endl;

			/*
				출력:
					11
					21
					31
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] std::array 의 장점" << std::endl;
			std::cout << "============================================" << std::endl;

			std::array<int, 5> values = { 1, 2, 3, 4, 5 };

			std::cout << "size = " << values.size() << std::endl;

			for (int i = 0; i < (int)values.size(); ++i)
			{
				std::cout << values[i] << ' ';
			}
			std::cout << std::endl << std::endl;

			/*
				설명:
				std::array 는 size() 로 길이를 쉽게 얻을 수 있다.
				이 점이 기본 배열보다 훨씬 편리하다.
			*/
		}

		system("pause");
	}
	
	void Test()
	{
		//arrays();

		//initializing_arrays();

		//accessing_the_values_of_an_array();

		//multidimensional_arrays();

		//arrays_of_strings();

		//arrays_as_parameters();

		//library_arrays();
	}

}// end of Arrays