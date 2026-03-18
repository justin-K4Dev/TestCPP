#include "stdafx.h"



namespace Operators
{
	void assignment_operator()
	{
		/*
			📚 대입 연산자 (Assignment operator) 
			
			(=)

			대입 연산자는 오른쪽의 값을 왼쪽 변수에 저장한다.

			형태:
				변수 = 값;
				변수1 = 변수2;

			예:
				x = 10;   // x에 10 저장
				a = b;    // b의 현재 값을 a에 복사

			중요한 점:
			1) 대입은 항상 오른쪽 -> 왼쪽으로 진행된다.
			2) 기존에 들어 있던 값은 새 값으로 덮어써진다.
			3) 변수끼리 대입하면 "연결"이 아니라 "복사"이다.
			4) 대입식 자체도 하나의 값으로 평가될 수 있다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] 가장 기본적인 대입" << std::endl;
			std::cout << "============================================" << std::endl;

			int x;      // 아직 초기화되지 않은 상태
			x = 5;      // x에 5를 저장

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				결과:
					x = 5

				설명:
				오른쪽 값 5가 왼쪽 변수 x에 저장된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 기존 값 덮어쓰기" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 3;  // x는 3
			std::cout << "초기 x = " << x << std::endl;

			x = 10;     // 기존 값 3은 사라지고 10으로 바뀜
			std::cout << "변경 후 x = " << x << std::endl;
			std::cout << std::endl;

			/*
				결과:
					초기 x = 3
					변경 후 x = 10

				설명:
				대입 연산은 기존 값을 유지하지 않고 새 값으로 덮어쓴다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 변수의 값을 다른 변수에 복사" << std::endl;
			std::cout << "============================================" << std::endl;

			int a, b;   // a: ?, b: ?
			a = 10;     // a: 10
			b = 4;      // b: 4
			a = b;      // a: 4, b: 4
			b = 7;      // a: 4, b: 7

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << std::endl;

			/*
				결과:
					a = 4
					b = 7

				설명:
				a = b; 를 실행한 순간,
				b의 값 4가 a에 복사되었다.

				그 이후 b를 7로 바꿔도
				a는 자동으로 바뀌지 않는다.

				즉:
				대입은 참조 연결이 아니라 값 복사이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 대입식도 하나의 값으로 사용 가능" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 0;
			int y = 0;

			y = 2 + (x = 5);

			std::cout << "x = " << x << std::endl;
			std::cout << "y = " << y << std::endl;
			std::cout << std::endl;

			/*
				결과:
					x = 5
					y = 7

				설명:
				(x = 5) 는 단순히 x에 5를 저장하는 것에서 끝나지 않고,
				그 식 자체도 값 5로 평가된다.

				따라서:
					y = 2 + (x = 5);
				는
					y = 2 + 5;
				와 같은 효과가 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] 연속 대입" << std::endl;
			std::cout << "============================================" << std::endl;

			int x, y, z;

			x = y = z = 5;

			std::cout << "x = " << x << std::endl;
			std::cout << "y = " << y << std::endl;
			std::cout << "z = " << z << std::endl;
			std::cout << std::endl;

			/*
				결과:
					x = 5
					y = 5
					z = 5

				설명:
				대입 연산자는 오른쪽에서 왼쪽으로 처리된다.

				즉,
					x = y = z = 5;
				는 내부적으로 다음과 비슷하다.

					z = 5;
					y = z;
					x = y;

				결과적으로 모두 5가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 6] 잘못 이해하기 쉬운 부분 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 100;
			int b = 200;

			a = b;      // a는 b의 값 200을 복사받음
			b = 999;    // b만 다시 변경

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << std::endl;

			/*
				결과:
					a = 200
					b = 999

				설명:
				많이 헷갈리는 부분이다.

				a = b; 를 했다고 해서
				a가 앞으로도 계속 b를 따라가는 것이 아니다.

				그 순간의 값만 복사한다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void arithmetic_operators()
	{
		/*
			📚 산술 연산자 (Arithmetic operator)

			( +, -, *, /, % )

			C++에서 기본적으로 제공하는 대표적인 산술 연산자는 다음 5개이다.

				연산자		의미
				---------------------
				+			덧셈
				-			뺄셈
				*			곱셈
				/			나눗셈
				%			나머지

			+, -, *, / 는 우리가 수학에서 사용하는 계산과 거의 같은 의미이다.

			단, / 와 % 는 자료형에 따라 결과가 다르게 보일 수 있으므로 주의가 필요하다.

			특히:
			1) 정수 / 정수  -> 정수 결과 (소수점 버림)
			2) % 는 나머지를 구하는 연산자
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] 덧셈 (+)" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 10;
			int b = 3;
			int result = a + b;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "a + b = " << result << std::endl;
			std::cout << std::endl;

			/*
				설명:
				10 + 3 = 13
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 뺄셈 (-)" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 10;
			int b = 3;
			int result = a - b;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "a - b = " << result << std::endl;
			std::cout << std::endl;

			/*
				설명:
				10 - 3 = 7
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 곱셈 (*)" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 10;
			int b = 3;
			int result = a * b;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "a * b = " << result << std::endl;
			std::cout << std::endl;

			/*
				설명:
				10 * 3 = 30
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 정수 나눗셈 (/)" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 10;
			int b = 3;
			int result = a / b;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "a / b = " << result << std::endl;
			std::cout << std::endl;

			/*
				설명:
				정수끼리 나누면 소수점 이하는 버린다.

				10 / 3 = 3.333...
				하지만 int / int 이므로 결과는 3
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] 실수 나눗셈 (/)" << std::endl;
			std::cout << "============================================" << std::endl;

			double a = 10.0;
			double b = 3.0;
			double result = a / b;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "a / b = " << result << std::endl;
			std::cout << std::endl;

			/*
				설명:
				실수끼리 나누면 소수점까지 포함한 결과가 나온다.

				10.0 / 3.0 = 3.33333...
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 6] 나머지 (%)" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 11;
			int b = 3;
			int result = a % b;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "a % b = " << result << std::endl;
			std::cout << std::endl;

			/*
				설명:
				11 / 3 = 3 ... 2

				몫은 3이고,
				나머지는 2이므로

				11 % 3 = 2
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 7] / 와 % 를 함께 보기" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 17;
			int b = 5;

			int quotient = a / b;   // 몫
			int remainder = a % b;  // 나머지

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "a / b = " << quotient << std::endl;
			std::cout << "a % b = " << remainder << std::endl;
			std::cout << std::endl;

			/*
				설명:
				17 / 5 = 3 ... 2

				즉,
				몫은 3
				나머지는 2
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 8] 홀수 / 짝수 판별에 % 사용" << std::endl;
			std::cout << "============================================" << std::endl;

			int value1 = 8;
			int value2 = 11;

			std::cout << "value1 = " << value1 << std::endl;
			if (value1 % 2 == 0)
			{
				std::cout << value1 << " 은(는) 짝수입니다." << std::endl;
			}
			else
			{
				std::cout << value1 << " 은(는) 홀수입니다." << std::endl;
			}

			std::cout << std::endl;

			std::cout << "value2 = " << value2 << std::endl;
			if (value2 % 2 == 0)
			{
				std::cout << value2 << " 은(는) 짝수입니다." << std::endl;
			}
			else
			{
				std::cout << value2 << " 은(는) 홀수입니다." << std::endl;
			}

			std::cout << std::endl;

			/*
				설명:
				어떤 수를 2로 나눴을 때 나머지가 0이면 짝수,
				1이면 홀수이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 9] 연산 결과를 변수에 다시 저장" << std::endl;
			std::cout << "============================================" << std::endl;

			int score = 50;

			std::cout << "초기 score = " << score << std::endl;

			score = score + 10;
			std::cout << "score = score + 10 -> " << score << std::endl;

			score = score - 20;
			std::cout << "score = score - 20 -> " << score << std::endl;

			score = score * 2;
			std::cout << "score = score * 2  -> " << score << std::endl;

			score = score / 4;
			std::cout << "score = score / 4  -> " << score << std::endl;

			std::cout << std::endl;

			/*
				설명:
				산술 연산의 결과를 다시 같은 변수에 저장할 수도 있다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void compound_assignment()
	{
		/*
			📚 복합 대입 연산자 (Compound Assignment Operator)
			
			( +=, -=, *=, /=, %=, >>=, <<=, &=, ^=, |= )

			복합 대입 연산자는
			"현재 변수값에 어떤 연산을 수행한 뒤,
			그 결과를 다시 같은 변수에 저장" 하는 연산자이다.

			즉 아래처럼 자주 쓰는 코드를

				y = y + x;

			더 짧게

				y += x;

			처럼 쓸 수 있다.

			대표적인 예:
				y += x;      -> y = y + x;
				x -= 5;      -> x = x - 5;
				x /= y;      -> x = x / y;
				price *= 2;  -> price = price * 2;

			산술 연산뿐 아니라
			비트 연산과 시프트 연산에도 같은 방식으로 사용할 수 있다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] += 기본 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int a, b = 3;
			a = b;      // a = 3

			a += 2;     // a = a + 2 = 5

			std::cout << "a = " << a << std::endl;
			std::cout << std::endl;

			/*
				결과:
					a = 5

				설명:
				a += 2; 는
				a = a + 2; 와 완전히 같다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] -= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 10;

			x -= 4;     // x = x - 4

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				10 - 4 = 6
				따라서 x는 6이 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] *= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 7;

			x *= 3;     // x = x * 3

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				7 * 3 = 21
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] /= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 20;

			x /= 3;     // x = x / 3

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				int / int 이므로 소수점 이하는 버려진다.

				20 / 3 = 6.666...
				하지만 정수 결과는 6
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] %= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 20;

			x %= 3;     // x = x % 3

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				20 / 3 = 6 ... 2
				나머지는 2 이므로 x는 2가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 6] <<= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 2;      // 0000 0010

			x <<= 1;        // x = x << 1 -> 0000 0100 = 4

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				왼쪽 시프트는 비트를 왼쪽으로 이동시킨다.
				2를 왼쪽으로 1칸 밀면 4가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 7] >>= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 16;     // 0001 0000

			x >>= 2;        // x = x >> 2 -> 0000 0100 = 4

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				오른쪽 시프트는 비트를 오른쪽으로 이동시킨다.
				16을 오른쪽으로 2칸 밀면 4가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 8] &= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 60;     // 0011 1100
			int y = 13;     // 0000 1101

			x &= y;         // x = x & y

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				0011 1100
				0000 1101
				---------
				0000 1100 = 12

				따라서 x는 12가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 9] |= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 60;     // 0011 1100
			int y = 13;     // 0000 1101

			x |= y;         // x = x | y

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				0011 1100
				0000 1101
				---------
				0011 1101 = 61

				따라서 x는 61이 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 10] ^= 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 60;     // 0011 1100
			int y = 13;     // 0000 1101

			x ^= y;         // x = x ^ y

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				0011 1100
				0000 1101
				---------
				0011 0001 = 49

				따라서 x는 49가 된다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 11] 여러 복합 대입 연산을 연속으로 사용" << std::endl;
			std::cout << "============================================" << std::endl;

			int score = 10;

			std::cout << "초기 score = " << score << std::endl;

			score += 5;   // 15
			std::cout << "score += 5  -> " << score << std::endl;

			score *= 2;   // 30
			std::cout << "score *= 2  -> " << score << std::endl;

			score -= 8;   // 22
			std::cout << "score -= 8  -> " << score << std::endl;

			score /= 3;   // 7
			std::cout << "score /= 3  -> " << score << std::endl;

			score %= 5;   // 2
			std::cout << "score %= 5  -> " << score << std::endl;

			std::cout << std::endl;

			/*
				설명:
				복합 대입 연산자는
				변수값을 계속 갱신해가며 계산할 때 매우 자주 사용된다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void increment_n_decrement()
	{
		/*
			📚 증가 / 감소 연산자 (Increment & Decrement Operator)
			
			(++, --)

			증가 연산자 ++ 는 변수의 값을 1 증가시킨다.
			감소 연산자 -- 는 변수의 값을 1 감소시킨다.

			예:
				++x;
				x += 1;
				x = x + 1;

			위 3개는 기능적으로 같은 의미이다.

			중요한 특징:
			++ 와 -- 는 변수 앞(prefix)에도 붙을 수 있고,
			변수 뒤(suffix)에도 붙을 수 있다.

				++x   // 전위 증가
				x++   // 후위 증가

			단독으로 쓸 때는 둘 다 결국 x를 1 증가시키지만,
			식의 결과값이 사용될 때는 차이가 있다.

			1) 전위 증가 (++x)
			   먼저 x를 증가시키고,
			   증가된 값을 식의 결과로 사용한다.

			2) 후위 증가 (x++)
			   현재 x 값을 먼저 식의 결과로 사용하고,
			   그 다음에 x를 증가시킨다.
		*/
		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] 전위 증가 (++x)" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 3;
			int y = ++x;

			std::cout << "x = " << x << std::endl;
			std::cout << "y = " << y << std::endl;
			std::cout << std::endl;

			/*
				설명:
				++x 는 먼저 x를 1 증가시킨다.
				그래서 x는 4가 되고,
				그 증가된 값 4가 y에 대입된다.

				결과:
					x = 4
					y = 4
			*/
		}
		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 후위 증가 (x++)" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 3;
			int y = x++;

			std::cout << "x = " << x << std::endl;
			std::cout << "y = " << y << std::endl;
			std::cout << std::endl;

			/*
				설명:
				x++ 는 먼저 현재 x 값을 식의 결과로 사용한다.
				그 후 x를 1 증가시킨다.

				즉,
				y에는 증가 전 값 3이 들어가고,
				그 다음 x가 4가 된다.

				결과:
					x = 4
					y = 3
			*/
		}
		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 후위 증가 출력 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 3;
			int y = ++x;

			std::cout << x++ << std::endl;
			std::cout << x << std::endl;
			std::cout << y << std::endl;
			std::cout << std::endl;

			/*
				설명:
				처음:
					x = 3
					y = ++x -> x=4, y=4

				그 다음:
					std::cout << x++;

				이 줄은 현재 x 값 4를 먼저 출력하고,
				출력 후 x를 5로 증가시킨다.

				따라서:
					첫 줄 출력: 4
					둘째 줄 출력: 5
					셋째 줄 출력: 4
			*/
		}
		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 감소 연산자 (--)" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 5;
			int a = --x; // 먼저 감소
			int y = 5;
			int b = y--; // 먼저 사용 후 감소

			std::cout << "전위 감소 결과 x = " << x << ", a = " << a << std::endl;
			std::cout << "후위 감소 결과 y = " << y << ", b = " << b << std::endl;
			std::cout << std::endl;

			/*
				설명:
				--x : 먼저 감소 후 사용
				y-- : 먼저 사용 후 감소
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void relational_n_comparison_operators()
	{
		/*
			📚 관계 / 비교 연산자 (Relational & Comparison Operator)

			( ==, !=, >, <, >=, <= )

			두 값을 비교해서
			결과를 true 또는 false 로 얻는 연산자이다.

				==   같다
				!=   다르다
				>    크다
				<    작다
				>=   크거나 같다
				<=   작거나 같다

			주의:
			=  는 대입 연산자
			== 는 비교 연산자

			둘은 완전히 다르다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] 기본 비교 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "(7 == 5) = " << (7 == 5) << std::endl;
			std::cout << "(5 > 4)  = " << (5 > 4) << std::endl;
			std::cout << "(3 != 2) = " << (3 != 2) << std::endl;
			std::cout << "(6 >= 6) = " << (6 >= 6) << std::endl;
			std::cout << "(5 < 5)  = " << (5 < 5) << std::endl;
			std::cout << std::endl;

			/*
				true 는 보통 1,
				false 는 보통 0 으로 출력된다.
			*/
		}
		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 변수 비교 예제" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 2;
			int b = 3;
			int c = 6;

			std::cout << "(a == 5)     = " << (a == 5) << std::endl;
			std::cout << "(a * b >= c) = " << (a * b >= c) << std::endl;
			std::cout << "(b + 4 > a * c) = " << (b + 4 > a * c) << std::endl;
			std::cout << std::endl;

			/*
				a == 5       -> 2 == 5      -> false
				a*b >= c     -> 2*3 >= 6    -> true
				b+4 > a*c    -> 3+4 > 2*6   -> false
			*/
		}
		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] = 와 == 차이" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 2;
			int b = 3;

			bool result = ((b = 2) == a);

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "result = " << result << std::endl;
			std::cout << std::endl;

			/*
				설명:
				(b = 2) 는 먼저 b에 2를 대입한다.
				그 다음 2 == a 를 비교한다.

				a도 2이므로 결과는 true

				최종:
					a = 2
					b = 2
					result = true(1)
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void logical_operators()
	{
		/*
			📚 논리 연산자 (Logical Operator)

			( !, &&, || )

			!   : NOT  -> 참/거짓 반전
			&&  : AND  -> 둘 다 true일 때만 true
			||  : OR   -> 둘 중 하나라도 true면 true

			결과는 항상 true 또는 false 이다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] NOT (!)" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "!(5 == 5) = " << !(5 == 5) << std::endl;
			std::cout << "!(6 <= 4) = " << !(6 <= 4) << std::endl;
			std::cout << "!true     = " << !true << std::endl;
			std::cout << "!false    = " << !false << std::endl;
			std::cout << std::endl;
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] AND (&&)" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "((5 == 5) && (3 > 6)) = " << ((5 == 5) && (3 > 6)) << std::endl;
			std::cout << "((5 == 5) && (6 > 3)) = " << ((5 == 5) && (6 > 3)) << std::endl;
			std::cout << std::endl;

			/*
				true && false -> false
				true && true  -> true
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] OR (||)" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "((5 == 5) || (3 > 6)) = " << ((5 == 5) || (3 > 6)) << std::endl;
			std::cout << "((4 < 1)  || (2 > 8)) = " << ((4 < 1) || (2 > 8)) << std::endl;
			std::cout << std::endl;

			/*
				true || false  -> true
				false || false -> false
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] short-circuit 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			int i = 10;
			int n = 20;

			if ((i < 10) && (++i < n))
			{
				std::cout << "조건 참" << std::endl;
			}
			else
			{
				std::cout << "조건 거짓" << std::endl;
			}

			std::cout << "i = " << i << std::endl;
			std::cout << std::endl;

			/*
				설명:
				왼쪽 조건 (i < 10) 은 false 이다.
				&& 는 왼쪽이 false 이면 전체 결과가 이미 false 이므로
				오른쪽 (++i < n) 은 아예 검사하지 않는다.

				그래서 i는 증가하지 않고 그대로 10이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] || short-circuit 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 5;

			if ((x == 5) || (++x > 0))
			{
				std::cout << "조건 참" << std::endl;
			}

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				왼쪽 조건 (x == 5) 가 true 이므로
				|| 는 오른쪽을 검사하지 않는다.

				따라서 ++x 는 실행되지 않고 x는 5 그대로이다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void conditional_ternary_operator()
	{
		/*
			📚 조건(삼항) 연산자 (Conditional Ternary Operator)
			( ? : )

			형태:
				조건식 ? 참일 때 값 : 거짓일 때 값

			의미:
			조건식이 true 이면 앞의 값을 선택하고,
			false 이면 뒤의 값을 선택한다.

			간단한 if-else 를 한 줄로 표현할 때 자주 사용한다.
		*/

		{
			int a, b, c;

			a = 2;
			b = 7;
			c = (a > b) ? a : b;

			std::cout << c << '\n';

			/*
				설명:
				(a > b) 는 (2 > 7) 이므로 false
				따라서 : 뒤의 값 b가 선택된다.

				결과:
					c = 7
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 큰 값 선택" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 10;
			int y = 20;

			int maxValue = (x > y) ? x : y;

			std::cout << "x = " << x << std::endl;
			std::cout << "y = " << y << std::endl;
			std::cout << "maxValue = " << maxValue << std::endl;
			std::cout << std::endl;
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 짝수 / 홀수 문자열 선택" << std::endl;
			std::cout << "============================================" << std::endl;

			int value = 11;
			const char* text = (value % 2 == 0) ? "짝수" : "홀수";

			std::cout << "value = " << value << std::endl;
			std::cout << "결과 = " << text << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void comma_operator()
	{
		/*
			📚 쉼표 연산자 (Comma Operator)
			( , )

			쉼표 연산자는 여러 식을 순서대로 실행하고,
			마지막 식의 결과를 전체 결과로 사용한다.

			형태:
				(식1, 식2)
				(식1, 식2, 식3)

			의미:
			1) 왼쪽 식부터 차례대로 실행
			2) 마지막 식의 값이 전체 식의 결과가 됨
		*/

		{
			int a(0), b(0);

			a = (b = 3, b + 2);

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << std::endl;

			/*
				설명:
				(b = 3, b + 2)

				1) 먼저 b = 3 실행
				2) 그 다음 b + 2 계산 -> 5
				3) 전체 괄호 식의 결과는 마지막 식의 값 5

				따라서:
					b = 3
					a = 5
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 여러 식 순서대로 실행" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 0;
			int y = 0;
			int z = 0;

			z = (x = 1, y = 2, x + y);

			std::cout << "x = " << x << std::endl;
			std::cout << "y = " << y << std::endl;
			std::cout << "z = " << z << std::endl;
			std::cout << std::endl;

			/*
				설명:
				1) x = 1
				2) y = 2
				3) x + y = 3

				마지막 식의 결과가 전체 결과이므로
				z = 3
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	/*
		8비트 형태로 출력하기 위한 헬퍼 함수
		- 예제를 보기 쉽게 하기 위해 하위 8비트만 출력
	*/
	void print8(const char* label, std::uint8_t value)
	{
		std::cout << std::left << std::setw(12) << label
			<< " = "
			<< std::bitset<8>(value)
			<< " (dec: " << std::right << std::setw(3) << static_cast<int>(value) << ")\n";
	}

	/*
		32비트 signed int 출력
		- NOT, signed shift 예제 확인용
	*/
	void print32(const char* label, std::int32_t value)
	{
		std::uint32_t u = static_cast<std::uint32_t>(value);

		std::cout << std::left << std::setw(12) << label
			<< " = "
			<< std::bitset<32>(u)
			<< " (dec: " << std::right << value << ")\n";
	}

	struct MaskApplier
	{
		// 하위 6비트만 유효한 영역으로 사용
		// 0x3F = 63 = 111111
		static const int FlagsMask = 0x3F;

		int Value;
		int Mask;

		MaskApplier(int value, int mask)
			: Value(value), Mask(mask)
		{
		}

		int getMaskedValue() const
		{
			// Value 전체를 쓰는 것이 아니라
			// Mask가 1인 비트만 남긴다.
			return Value & Mask;
		}

		int apply(int area) const
		{
			// 핵심 공식
			// 1) Value에서 Mask가 1인 비트만 추출
			// 2) area에서 Mask가 0인 비트만 유지
			// 3) 둘을 OR로 합쳐 최종 결과 생성
			//
			// 즉,
			// - Mask가 1인 비트 -> Value 사용
			// - Mask가 0인 비트 -> area 유지
			return (Value & Mask) | (area & ~Mask);
		}
	};

	void bitwise_operators()
	{
		/*
			📚 비트 연산자 (Bitwise Operator)

			비트 연산자는 변수에 저장된 값을 "숫자" 자체로 보는 것이 아니라
			그 숫자를 메모리에서 표현하는 "비트 패턴" 기준으로 연산한다.

			연산자		의미				설명
			------------------------------------------------------------------
			&			비트 AND			둘 다 1일 때만 1
			|			비트 OR				둘 중 하나라도 1이면 1
			^			비트 XOR			서로 다를 때만 1
			~			비트 NOT			비트를 반전시킴 (0->1, 1->0)
			<<			왼쪽 시프트			비트를 왼쪽으로 이동
			>>			오른쪽 시프트		비트를 오른쪽으로 이동


			------------------------------------------------------------------------------------------------
			1) AND (&)
			------------------------------------------------------------------------------------------------
				a = 00101100	(44)
				b = 00001111	(15)
				------------- &
					00001100	(12)

				- 두 비트가 모두 1인 자리만 1
				- 특정 비트 추출(masking)에 자주 사용

			------------------------------------------------------------------------------------------------
			2) OR (|)
			------------------------------------------------------------------------------------------------
				a = 00101100	(44)
				b = 00001111	(15)
				------------- |
					00101111	(47)

				- 둘 중 하나라도 1이면 1
				- 특정 비트를 켜는 데 사용

			------------------------------------------------------------------------------------------------
			3) XOR (^)
			------------------------------------------------------------------------------------------------
				a = 00101100	(44)
				b = 00001111	(15)
				------------- ^
					00100011	(35)

				- 두 비트가 다를 때만 1
				- 비트 토글, 차이 비교에 자주 사용

			------------------------------------------------------------------------------------------------
			4) NOT (~)
			------------------------------------------------------------------------------------------------
				a = 00101100	(44)
				------------- ~
					11010011	(211, uint8_t 기준)

				또는 32비트 int 기준:
				a = 00000000 00000000 00000000 00101100    (44)
				~a= 11111111 11111111 11111111 11010011    (-45)

				- 모든 비트를 반전
				- 단, 실제 int에서는 자료형 크기 전체(예: 32비트)가 반전됨

			------------------------------------------------------------------------------------------------
			5) 왼쪽 시프트 (<<)
			------------------------------------------------------------------------------------------------
				a = 00000101   (5)

				a << 1
				-------------
					00001010   (10)

				a << 2
				-------------
					00010100   (20)

				- 비트를 왼쪽으로 밀고 오른쪽 빈칸은 0으로 채움
				- 일반적으로 2배, 4배 같은 효과를 냄
				- 단, 범위를 넘는 시프트는 주의 필요

			------------------------------------------------------------------------------------------------
			6) 오른쪽 시프트 (>>)
			------------------------------------------------------------------------------------------------
				a = 00010100   (20)

				a >> 1
				-------------
					00001010   (10)

				a >> 2
				-------------
					00000101   (5)

				- 비트를 오른쪽으로 밀어냄
				- unsigned는 왼쪽 빈칸이 0으로 채워짐
				- signed는 컴파일러/구현에 따라 부호 확장 여부를 고려해야 함
		*/
		{
			//-------------------------------------------------------------------------------------
			// AND (&) 연산자
			//   - 둘 다 1인 자리만 1
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== AND (&) ==========\n";

				std::uint8_t a = 0b00101100; // 44
				std::uint8_t b = 0b00001111; // 15
				std::uint8_t r = a & b;

				print8("a", a);
				print8("b", b);
				print8("a & b", r);

				/*
					a     = 00101100
					b     = 00001111
					----------------- &
							00001100
				*/
			}

			//-------------------------------------------------------------------------------------
			// OR (|) 연산자
			//	 - 둘 중 하나라도 1이면 1
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== OR (|) ==========\n";

				std::uint8_t a = 0b00101100; // 44
				std::uint8_t b = 0b00001111; // 15
				std::uint8_t r = a | b;

				print8("a", a);
				print8("b", b);
				print8("a | b", r);

				/*
					a     = 00101100
					b     = 00001111
					----------------- |
							00101111
				*/
			}

			//-------------------------------------------------------------------------------------
			// XOR (^) 연산자
			//   - 서로 다를 때만 1
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== XOR (^) ==========\n";

				std::uint8_t a = 0b00101100; // 44
				std::uint8_t b = 0b00001111; // 15
				std::uint8_t r = a ^ b;

				print8("a", a);
				print8("b", b);
				print8("a ^ b", r);

				/*
					a     = 00101100
					b     = 00001111
					----------------- ^
							00100011
				*/
			}

			//-------------------------------------------------------------------------------------
			// NOT (~) 연산자
			//   - 모든 비트를 반전
			//	 - uint8_t와 int32_t 둘 다 보여줌
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== NOT (~) ==========\n";

				std::uint8_t a8 = 0b00101100;       // 44
				std::uint8_t r8 = static_cast<std::uint8_t>(~a8);

				print8("a8", a8);
				print8("~a8", r8);

				/*
					a8    = 00101100
					----------------- ~
							11010011
				*/

				std::cout << "\n[32비트 int 기준]\n";

				std::int32_t a32 = 44;
				std::int32_t r32 = ~a32;

				print32("a32", a32);
				print32("~a32", r32);

				/*
					a32  = 00000000 00000000 00000000 00101100
					~a32 = 11111111 11111111 11111111 11010011
				*/
			}

			//-------------------------------------------------------------------------------------
			// 왼쪽 시프트 (<<) 연산자
			//   - 비트를 왼쪽으로 이동
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== LEFT SHIFT (<<) ==========\n";

				std::uint8_t a = 0b00000101; // 5
				std::uint8_t r1 = static_cast<std::uint8_t>(a << 1);
				std::uint8_t r2 = static_cast<std::uint8_t>(a << 2);

				print8("a", a);
				print8("a << 1", r1);
				print8("a << 2", r2);

				/*
					a      = 00000101
					a << 1 = 00001010
					a << 2 = 00010100
				*/
			}

			//-------------------------------------------------------------------------------------
			//  오른쪽 시프트 (>>) 테스트 - unsigned
			//    - 왼쪽이 0으로 채워짐
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== RIGHT SHIFT (>>) unsigned ==========\n";

				std::uint8_t a = 0b00010100; // 20
				std::uint8_t r1 = static_cast<std::uint8_t>(a >> 1);
				std::uint8_t r2 = static_cast<std::uint8_t>(a >> 2);

				print8("a", a);
				print8("a >> 1", r1);
				print8("a >> 2", r2);

				/*
					a      = 00010100
					a >> 1 = 00001010
					a >> 2 = 00000101
				*/
			}

			//-------------------------------------------------------------------------------------
			// 오른쪽 시프트 (>>) 테스트 - signed
			//  - 음수의 경우 부호 확장 여부를 관찰
			//  - 대부분 환경에서는 산술 시프트처럼 동작
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== RIGHT SHIFT (>>) signed ==========\n";

				std::int32_t a = -20;
				std::int32_t r1 = a >> 1;
				std::int32_t r2 = a >> 2;

				print32("a", a);
				print32("a >> 1", r1);
				print32("a >> 2", r2);

				/*
					예시(대부분 2의 보수 + 산술 시프트 환경):
					a      = 11111111 11111111 11111111 11101100   (-20)
					a >> 1 = 11111111 11111111 11111111 11110110   (-10)
					a >> 2 = 11111111 11111111 11111111 11111011   (-5)

					signed right shift는 구현 특성을 고려해야 함
				*/
			}

			//-------------------------------------------------------------------------------------
			// 마스킹(masking) 예제
			//   - 특정 비트만 추출
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== MASKING EXAMPLE ==========\n";

				std::uint8_t value = 0b10110110;
				std::uint8_t mask = 0b00001111;
				std::uint8_t r = value & mask;

				print8("value", value);
				print8("mask", mask);
				print8("value&mask", r);

				/*
					value      = 10110110
					mask       = 00001111
					---------------------- &
								 00000110

					-> 하위 4비트만 추출
				*/
			}

			//-------------------------------------------------------------------------------------
			// 비트 켜기(set) 예제
			//   - OR로 특정 비트를 1로 설정
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== SET BIT EXAMPLE ==========\n";

				std::uint8_t value = 0b00100010;
				std::uint8_t mask = 0b00000100;
				std::uint8_t r = value | mask;

				print8("value", value);
				print8("mask", mask);
				print8("value|mask", r);

				/*
					value      = 00100010
					mask       = 00000100
					---------------------- |
								 00100110

					-> 2번 비트를 강제로 켬
				*/
			}

			//-------------------------------------------------------------------------------------
			// 비트 토글(toggle) 예제
			//  - XOR로 특정 비트를 뒤집음
			//-------------------------------------------------------------------------------------
			{
				std::cout << "\n========== TOGGLE BIT EXAMPLE ==========\n";

				std::uint8_t value = 0b00100110;
				std::uint8_t mask = 0b00000100;
				std::uint8_t r = value ^ mask;

				print8("value", value);
				print8("mask", mask);
				print8("value^mask", r);

				/*
					value      = 00100110
					mask       = 00000100
					---------------------- ^
								 00100010

					-> 2번 비트를 반전
				*/
			}
		}


		auto toBinary8 = [](int value) -> std::string
		{
			return std::bitset<8>(value & 0xFF).to_string();
		};

		auto toBinary6 = [](int value) -> std::string
		{
			return std::bitset<6>(value & MaskApplier::FlagsMask).to_string();
		};

		auto printLine = []()
		{
			std::cout << "------------------------------------------------------------" << std::endl;
		};

		auto runMask = [&](const std::string& title, int area, int value, int mask, int expected)
		{
			MaskApplier applier(value, mask);

			int maskedValue = applier.getMaskedValue();
			int result = applier.apply(area);

			std::cout << title << std::endl;
			printLine();

			std::cout << "area         : " << area << "  (" << toBinary6(area) << ")" << std::endl;
			std::cout << "value        : " << value << "  (" << toBinary6(value) << ")" << std::endl;
			std::cout << "mask         : " << mask << "  (" << toBinary6(mask) << ")" << std::endl;
			std::cout << "maskedValue  : " << maskedValue << "  (" << toBinary6(maskedValue) << ")" << std::endl;
			std::cout << "result       : " << result << "  (" << toBinary6(result) << ")" << std::endl;
			std::cout << "expected     : " << expected << "  (" << toBinary6(expected) << ")" << std::endl;
			std::cout << "PASS         : " << (result == expected ? "OK" : "FAIL") << std::endl;
			std::cout << std::endl;
		};

		{
			printLine();
			std::cout << "[1] 왼쪽 시프트 (<<) / 오른쪽 시프트 (>>)" << std::endl;
			printLine();

			int a = 2;

			// 2 = 0000 0010
			std::cout << "초기 a       : " << a << " (" << toBinary8(a) << ")" << std::endl;

			a = a << 1;
			// 0000 0010 -> 0000 0100
			// 2 -> 4
			std::cout << "a << 1       : " << a << " (" << toBinary8(a) << ")" << std::endl;

			a = a << 2;
			// 0000 0100 -> 0001 0000
			// 4 -> 16
			std::cout << "a << 2       : " << a << " (" << toBinary8(a) << ")" << std::endl;

			a = a >> 1;
			// 0001 0000 -> 0000 1000
			// 16 -> 8
			std::cout << "a >> 1       : " << a << " (" << toBinary8(a) << ")" << std::endl;

			std::cout << std::endl;
		}

		{
			printLine();
			std::cout << "[2] &, |, ^, ~ 기본 예제" << std::endl;
			printLine();

			int a = 60; // 0011 1100
			int b = 13; // 0000 1101

			std::cout << "a            : " << a << " (" << toBinary8(a) << ")" << std::endl;
			std::cout << "b            : " << b << " (" << toBinary8(b) << ")" << std::endl;
			std::cout << std::endl;

			int andResult = a & b;
			// 0011 1100
			// 0000 1101
			// ----------
			// 0000 1100 = 12
			std::cout << "a & b        : " << andResult << " (" << toBinary8(andResult) << ")" << std::endl;

			int orResult = a | b;
			// 0011 1100
			// 0000 1101
			// ----------
			// 0011 1101 = 61
			std::cout << "a | b        : " << orResult << " (" << toBinary8(orResult) << ")" << std::endl;

			int xorResult = a ^ b;
			// 0011 1100
			// 0000 1101
			// ----------
			// 0011 0001 = 49
			std::cout << "a ^ b        : " << xorResult << " (" << toBinary8(xorResult) << ")" << std::endl;

			int notA = ~a;
			// 모든 비트를 반전
			std::cout << "~a           : " << notA << " (" << toBinary8(notA) << ")" << std::endl;
			std::cout << std::endl;
		}

		{
			printLine();
			std::cout << "[3] 플래그 상태 관리 예제" << std::endl;
			printLine();

			const unsigned int STATE_NONE = 0;
			const unsigned int STATE_CONNECTED = 1 << 0; // 0001
			const unsigned int STATE_LOGINED = 1 << 1; // 0010
			const unsigned int STATE_IN_ROOM = 1 << 2; // 0100
			const unsigned int STATE_READY = 1 << 3; // 1000

			unsigned int userState = STATE_NONE;

			// | : 상태 추가
			userState |= STATE_CONNECTED;
			userState |= STATE_LOGINED;

			std::cout << "현재 userState         : " << userState << " (" << std::bitset<4>(userState) << ")" << std::endl;

			// & : 상태 포함 여부 검사
			if (userState & STATE_CONNECTED)
			{
				std::cout << "STATE_CONNECTED 포함" << std::endl;
			}

			if (userState & STATE_LOGINED)
			{
				std::cout << "STATE_LOGINED 포함" << std::endl;
			}

			if ((userState & STATE_IN_ROOM) == 0)
			{
				std::cout << "STATE_IN_ROOM 없음" << std::endl;
			}

			// ^ : 토글
			userState ^= STATE_READY;
			std::cout << "READY 토글 ON          : " << userState << " (" << std::bitset<4>(userState) << ")" << std::endl;

			userState ^= STATE_READY;
			std::cout << "READY 토글 OFF         : " << userState << " (" << std::bitset<4>(userState) << ")" << std::endl;

			std::cout << std::endl;
		}

		{
			printLine();
			std::cout << "[4] XOR를 이용한 변경 감지 예제" << std::endl;
			printLine();

			const unsigned int FLAG_HP_CHANGED = 1 << 0;
			const unsigned int FLAG_MP_CHANGED = 1 << 1;
			const unsigned int FLAG_POS_CHANGED = 1 << 2;
			const unsigned int FLAG_LEVEL_CHANGED = 1 << 3;

			unsigned int oldState = FLAG_HP_CHANGED | FLAG_POS_CHANGED; // 0101
			unsigned int newState = FLAG_MP_CHANGED | FLAG_POS_CHANGED; // 0110

			unsigned int changed = oldState ^ newState;

			std::cout << "oldState     : " << oldState << " (" << std::bitset<4>(oldState) << ")" << std::endl;
			std::cout << "newState     : " << newState << " (" << std::bitset<4>(newState) << ")" << std::endl;
			std::cout << "changed      : " << changed << " (" << std::bitset<4>(changed) << ")" << std::endl;

			if (changed & FLAG_HP_CHANGED)    std::cout << "HP 상태 변경됨" << std::endl;
			if (changed & FLAG_MP_CHANGED)    std::cout << "MP 상태 변경됨" << std::endl;
			if (changed & FLAG_POS_CHANGED)   std::cout << "Position 상태 변경됨" << std::endl;
			if (changed & FLAG_LEVEL_CHANGED) std::cout << "Level 상태 변경됨" << std::endl;

			std::cout << std::endl;
		}

		{
			printLine();
			std::cout << "[5] MaskApplier 테스트" << std::endl;
			printLine();

			// TEST 1
			// 하위 3비트만 교체
			// area  = 56 = 111000
			// value =  5 = 000101
			// mask  =  7 = 000111
			// result= 61 = 111101
			runMask(
				"TEST 1 - 하위 3비트만 교체",
				56,
				5,
				7,
				61
			);

			// TEST 2
			// 상위 3비트만 교체
			// area  =  7 = 000111
			// value = 40 = 101000
			// mask  = 56 = 111000
			// result= 47 = 101111
			runMask(
				"TEST 2 - 상위 3비트만 교체",
				7,
				40,
				56,
				47
			);

			// TEST 3
			// 전체 6비트 교체
			runMask(
				"TEST 3 - 전체 6비트 교체",
				0,
				63,
				MaskApplier::FlagsMask,
				63
			);

			// TEST 4
			// mask가 0이면 아무 것도 바뀌지 않음
			runMask(
				"TEST 4 - mask가 0이면 원본 유지",
				45,
				18,
				0,
				45
			);

			// TEST 5
			// value에 mask 바깥 비트가 있어도 실제 적용은 mask 안쪽만 된다.
			runMask(
				"TEST 5 - Value는 Mask 범위 안쪽만 적용",
				0,
				63,
				3,
				3
			);

			// TEST 6
			// 중간 비트만 선택적으로 교체
			runMask(
				"TEST 6 - 중간 비트만 교체",
				51,
				12,
				12,
				63
			);
		}

		{
			/*
				📌 마스크(mask)와 ~mask

				mask
				- 남기고 싶은 비트가 1
				- 추출용
				- value & mask 형태로 사용
				- mask에서 1인 비트만 결과에 남고, 0인 비트는 제거된다.

				예:
					value  = 11010110
					mask   = 00001111
					result = value & mask
						   = 00000110

				~mask
				- 지우고 싶은 비트가 0
				- 제거용
				- value & ~mask 형태로 사용
				- 원래 mask에서 1이었던 비트를 제거한다.

				예:
					value  = 11010110
					mask   = 00001111
					~mask  = 11110000
					result = value & ~mask
						   = 11010000

				정리
				- value & mask  : mask가 1인 비트만 남긴다.   (추출)
				- value & ~mask : mask가 1인 비트를 제거한다. (제거)

				[주의]
				- int 는 보통 32비트라서 실제 ~mask 는 이렇게 된다.
				  mask   = 00000000 00000000 00000000 00001111 =>  15
				  ~mask  = 11111111 11111111 11111111 11110000 => -16
				  출력하면 음수가 나올 수도 있다 !!!.

				  ~mask  = 00000000 00000000 00000000 11110000 => 240

				  1 × 2^7 = 128
				  1 × 2^6 = 64
				  1 × 2^5 = 32
				  1 × 2^4 = 16
				  0 × 2^3 = 0
				  0 × 2^2 = 0
				  0 × 2^1 = 0
				  0 × 2^0 = 0
				  ------------------------
				  128 + 64 + 32 + 16 = 240
				  
				  그래서 아래와 같이 0xFF 를 이용하는 방법도 있다 !!!
				  ~mask  = 11111111 11111111 11111111 11110000 => -16
				  0xFF   = 00000000 00000000 00000000 11111111 => 255
				  (~mask & 0xFF) => 240
			*/

			printLine();
			std::cout << "mask / ~mask 테스트" << std::endl;
			printLine();

			int value = 0b11010110;
			int mask = 0b00001111;

			int extracted = value & mask;
			int removed = value & ~mask;

			std::cout << "value        : " << value << " (" << toBinary8(value) << ")" << std::endl;
			std::cout << "mask         : " << mask << " (" << toBinary8(mask) << ")" << std::endl;
			std::cout << "~mask        : " << (~mask) << " (" << toBinary8(~mask) << ")" << std::endl;
			std::cout << std::endl;

			// mask: 1인 비트만 남긴다.
			// 11010110
			// 00001111
			// --------
			// 00000110
			std::cout << "value & mask : " << extracted << " (" << toBinary8(extracted) << ")" << std::endl;

			// ~mask: mask의 비트를 반전
			// mask  = 00001111
			// ~mask = 11110000   (toBinary8 기준 하위 8비트만 표시)
			//
			// 11010110
			// 11110000
			// --------
			// 11010000
			std::cout << "value & ~mask: " << removed << " (" << toBinary8(removed) << ")" << std::endl;

			std::cout << std::endl;

			// 추가 예제 1: 하위 4비트만 추출
			int lower4 = value & 0b00001111;
			std::cout << "하위 4비트 추출      : " << lower4 << " (" << toBinary8(lower4) << ")" << std::endl;

			// 추가 예제 2: 하위 4비트 제거
			int clearLower4 = value & ~0b00001111;
			std::cout << "하위 4비트 제거      : " << clearLower4 << " (" << toBinary8(clearLower4) << ")" << std::endl;

			// 추가 예제 3: 상위 4비트만 추출
			int upper4 = value & 0b11110000;
			std::cout << "상위 4비트 추출      : " << upper4 << " (" << toBinary8(upper4) << ")" << std::endl;

			std::cout << std::endl;


			//-------------------------------------------------------------------------------------
			// ~mask 의 실제 값과 0xFF 적용 결과 확인
			//-------------------------------------------------------------------------------------
			printLine();
			std::cout << "~mask / 0xFF 확인" << std::endl;
			printLine();

			int notMask = ~mask;
			int low8OfNotMask = (~mask) & 0xFF;
			int ff = 0xFF;

			std::cout << "mask                  : " << mask << std::endl;
			std::cout << "~mask                 : " << notMask << std::endl;
			std::cout << "0xFF                  : " << ff << " (" << toBinary8(ff) << ")" << std::endl;
			std::cout << "(~mask) & 0xFF        : " << low8OfNotMask << " (" << toBinary8(low8OfNotMask) << ")" << std::endl;
			std::cout << std::endl;

			// 설명용 계산 확인
			// mask   = 00000000 00000000 00000000 00001111 = 15
			// ~mask  = 11111111 11111111 11111111 11110000 = -16
			// 0xFF   = 00000000 00000000 00000000 11111111 = 255
			// ------------------------------------------------
			// 결과   = 00000000 00000000 00000000 11110000 = 240

			std::cout << "검증 1 : mask == 15            -> " << (mask == 15 ? "OK" : "FAIL") << std::endl;
			std::cout << "검증 2 : ~mask == -16         -> " << ((~mask) == -16 ? "OK" : "FAIL") << std::endl;
			std::cout << "검증 3 : ((~mask)&0xFF)==240  -> " << (((~mask) & 0xFF) == 240 ? "OK" : "FAIL") << std::endl;

			std::cout << std::endl;

			//-------------------------------------------------------------------------------------
			// 마스크 패턴 비교
			//-------------------------------------------------------------------------------------
			printLine();
			std::cout << "다양한 mask 패턴" << std::endl;
			printLine();

			int sample = 0b10101100;

			int maskLow4 = 0b00001111;
			int maskHigh4 = 0b11110000;
			int maskMiddle = 0b00111100;

			std::cout << "sample                : " << sample << " (" << toBinary8(sample) << ")" << std::endl;
			std::cout << std::endl;

			std::cout << "sample & low4Mask     : " << (sample & maskLow4)
				<< " (" << toBinary8(sample & maskLow4) << ")" << std::endl;

			std::cout << "sample & ~low4Mask    : " << (sample & ~maskLow4)
				<< " (" << toBinary8(sample & ~maskLow4) << ")" << std::endl;

			std::cout << "sample & high4Mask    : " << (sample & maskHigh4)
				<< " (" << toBinary8(sample & maskHigh4) << ")" << std::endl;

			std::cout << "sample & ~high4Mask   : " << (sample & ~maskHigh4)
				<< " (" << toBinary8(sample & ~maskHigh4) << ")" << std::endl;

			std::cout << "sample & middleMask   : " << (sample & maskMiddle)
				<< " (" << toBinary8(sample & maskMiddle) << ")" << std::endl;

			std::cout << "sample & ~middleMask  : " << (sample & ~maskMiddle)
				<< " (" << toBinary8(sample & ~maskMiddle) << ")" << std::endl;

			std::cout << std::endl;
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void explicit_type_casting_operator()
	{
		/*
			📚 명시적 형변환 연산자 (Explicit Type Casting Operator)

			형변환(casting)은
			어떤 자료형의 값을 다른 자료형으로 바꾸는 것이다.

			예를 들어:
				float -> int
				double -> int
				int -> float

			C++에서는 여러 가지 형변환 방법이 있지만,
			기본적으로 초보 단계에서 많이 보는 방법은 다음 2가지이다.

			1) C 스타일 형변환
				(타입)값

				예:
					i = (int)f;

			2) 함수형 표기(functional notation)
				타입(값)

				예:
					i = int(f);

			두 방식 모두 C++에서 사용할 수 있다.

			중요:
			실수 -> 정수 형변환을 하면
			소수점 이하 부분은 버려진다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] float -> int (C 스타일 형변환)" << std::endl;
			std::cout << "============================================" << std::endl;

			int i(0);
			float f = 3.14f;

			i = (int)f;

			std::cout << "f = " << f << std::endl;
			std::cout << "i = " << i << std::endl;
			std::cout << std::endl;

			/*
				설명:
				3.14f 를 int 로 바꾸면
				정수 부분만 남고 소수점 이하는 버려진다.

				따라서:
					i = 3
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] float -> int (함수형 표기)" << std::endl;
			std::cout << "============================================" << std::endl;

			int i(0);
			float f = 9.87f;

			i = int(f);

			std::cout << "f = " << f << std::endl;
			std::cout << "i = " << i << std::endl;
			std::cout << std::endl;

			/*
				설명:
				int(f) 도 (int)f 와 같은 의미이다.
				9.87 -> 9
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] int -> float" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 10;
			float b = (float)a;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << std::endl;

			/*
				설명:
				정수를 실수로 바꾸면
				값은 그대로이고 자료형만 float 로 바뀐다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 형변환이 없을 때와 있을 때의 차이" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 10;
			int b = 3;

			float result1 = a / b;          // int / int -> 3
			float result2 = (float)a / b;   // float / int -> 3.333...

			std::cout << "result1 = " << result1 << std::endl;
			std::cout << "result2 = " << result2 << std::endl;
			std::cout << std::endl;

			/*
				설명:
				a / b 는 int / int 이므로 먼저 정수 나눗셈이 수행된다.
				즉 10 / 3 = 3

				그 결과 3이 float 에 저장되어 3.0 이 된다.

				반면 (float)a / b 는
				float / int 이므로 실수 나눗셈이 되어
				3.333... 이 나온다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void sizeof_operator()
	{
		/*
			📚 sizeof 연산자 (SizeOf Operator)

			sizeof 는 어떤 자료형(type) 또는 변수(object)가
			메모리에서 몇 바이트(byte)를 차지하는지 알려주는 연산자이다.

			형태:
				sizeof(자료형)
				sizeof(변수)

			예:
				sizeof(char)
				sizeof(int)
				sizeof(x)

			중요:
			sizeof 의 결과는 실행 중에 계산되는 것이 아니라
			컴파일 시점에 결정되는 상수값이다.
		*/

		{
			int x = sizeof(char);

			std::cout << x << std::endl;

			/*
				결과:
					1

				설명:
				char 는 1바이트 자료형이다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 기본 자료형 크기 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "sizeof(char)   = " << sizeof(char) << std::endl;
			std::cout << "sizeof(short)  = " << sizeof(short) << std::endl;
			std::cout << "sizeof(int)    = " << sizeof(int) << std::endl;
			std::cout << "sizeof(long)   = " << sizeof(long) << std::endl;
			std::cout << "sizeof(float)  = " << sizeof(float) << std::endl;
			std::cout << "sizeof(double) = " << sizeof(double) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				자료형에 따라 차지하는 메모리 크기가 다르다.

				단, long 의 크기처럼
				일부 자료형은 컴파일러 / 플랫폼에 따라 달라질 수 있다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 변수로 sizeof 사용" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 10;
			double b = 3.14;
			char c = 'A';

			std::cout << "sizeof(a) = " << sizeof(a) << std::endl;
			std::cout << "sizeof(b) = " << sizeof(b) << std::endl;
			std::cout << "sizeof(c) = " << sizeof(c) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				sizeof 는 자료형뿐 아니라 변수에도 사용할 수 있다.
				이 경우 해당 변수의 자료형 크기를 반환한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 배열 크기 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			int arr[5] = { 10, 20, 30, 40, 50 };

			std::cout << "sizeof(arr) = " << sizeof(arr) << std::endl;
			std::cout << "sizeof(arr[0]) = " << sizeof(arr[0]) << std::endl;
			std::cout << "배열 원소 개수 = " << sizeof(arr) / sizeof(arr[0]) << std::endl;
			std::cout << std::endl;

			/*
				설명:
				arr 전체 크기 = int 5개 크기
				arr[0] 크기   = int 1개 크기

				따라서:
					배열 전체 크기 / 원소 하나 크기
				로 원소 개수를 구할 수 있다.
			*/
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void other_operators()
	{
		/*
			📚 기타 연산자 (Other operators)

			C++에는 지금까지 배운 연산자 외에도
			나중에 배우게 될 다양한 연산자들이 있다.

			예를 들면:
			1) 포인터 관련 연산자
				&   주소 얻기
				*   역참조

			2) 멤버 접근 연산자
				.   객체 멤버 접근
				->  포인터를 통한 멤버 접근

			3) 범위 지정 연산자
				::  스코프 지정

			4) 동적 메모리 연산자
				new
				delete

			5) 배열 / 함수 호출 관련 연산자
				[]
				()

			이 함수는 실제 계산보다는
			"앞으로 더 많은 연산자를 배우게 된다" 는 의미의 안내용이다.
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[안내] 앞으로 배우게 될 기타 연산자들" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "&   : 변수의 주소를 구할 때 사용" << std::endl;
			std::cout << "*   : 포인터가 가리키는 값을 접근할 때 사용" << std::endl;
			std::cout << ".   : 객체의 멤버 접근" << std::endl;
			std::cout << "->  : 포인터를 통한 객체 멤버 접근" << std::endl;
			std::cout << "::  : 범위(스코프) 지정" << std::endl;
			std::cout << "new / delete : 동적 메모리 할당 / 해제" << std::endl;
			std::cout << "[]  : 배열 인덱스 접근" << std::endl;
			std::cout << "()  : 함수 호출" << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	//---------------------------------------------------------------------------------------------

	void precedence_of_operators()
	{
		/*
			📚 연산자 우선순위 (Precedence of operators)

			하나의 식 안에 여러 연산자가 들어가면
			어떤 연산을 먼저 계산할지 순서가 필요하다.
			이 순서를 연산자 우선순위라고 한다.

			예:
				x = 5 + 7 % 2;

			여기서 % 는 + 보다 우선순위가 높기 때문에
			먼저 7 % 2 가 계산된다.

				7 % 2 = 1
				5 + 1 = 6

			따라서 x = 6 이 된다.

			괄호 () 를 사용하면
			기본 우선순위를 바꿀 수 있다.

			우선순위가 높은 것부터 낮은 것 순서로,
			C++ 연산자는 다음과 같은 순서로 평가된다.

			단계	우선순위 그룹					연산자					설명									결합 방향
			-----------------------------------------------------------------------------------------------------------------------
			1		범위 지정						::						범위 지정 연산자						왼쪽 → 오른쪽
			2		후위 연산자 (단항)				++ --					후위 증가 / 후위 감소					왼쪽 → 오른쪽
													()						함수 호출 / 함수형 표기
													[]						배열 첨자 접근
													.  ->					멤버 접근
			3		전위 연산자 (단항)				++ --					전위 증가 / 전위 감소					오른쪽 → 왼쪽
													~  !					비트 NOT / 논리 NOT
													+  -					단항 부호 연산자
													&  *					주소 연산 / 역참조
													new delete				메모리 할당 / 해제
													sizeof					크기 구하기
													(type)					C 스타일 형변환
			4		멤버 포인터						.*  ->*					멤버 포인터 접근						왼쪽 → 오른쪽
			5		산술 연산: 곱셈 계열			*  /  %					곱셈, 나눗셈, 나머지					왼쪽 → 오른쪽
			6		산술 연산: 덧셈 계열			+  -					덧셈, 뺄셈								왼쪽 → 오른쪽
			7		비트 시프트						<<  >>					왼쪽 시프트, 오른쪽 시프트				왼쪽 → 오른쪽
			8		관계 연산						<  >  <=  >=			크기 비교 연산자						왼쪽 → 오른쪽
			9		동등 비교						==  !=					같다 / 다르다							왼쪽 → 오른쪽
			10		비트 AND						&						비트 AND								왼쪽 → 오른쪽
			11		비트 XOR						^						비트 XOR								왼쪽 → 오른쪽
			12		비트 OR							|						비트 OR									왼쪽 → 오른쪽
			13		논리 AND						&&						논리 AND								왼쪽 → 오른쪽
			14		논리 OR							||						논리 OR									왼쪽 → 오른쪽
			15		대입 계열 식					=  *=  /=  %=  +=  -=	대입 / 복합 대입						오른쪽 → 왼쪽
													>>= <<= &= ^= |=
													?:						조건(삼항) 연산자
			16		쉼표 연산자						,						쉼표 구분자
		*/

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 1] % 가 + 보다 먼저 계산됨" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 5 + 7 % 2;

			std::cout << "x = " << x << std::endl;
			std::cout << std::endl;

			/*
				설명:
				7 % 2 = 1
				5 + 1 = 6

				따라서 x = 6
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] 괄호로 우선순위 변경" << std::endl;
			std::cout << "============================================" << std::endl;

			int x1 = 5 + (7 % 2);
			int x2 = (5 + 7) % 2;

			std::cout << "x1 = " << x1 << std::endl;
			std::cout << "x2 = " << x2 << std::endl;
			std::cout << std::endl;

			/*
				설명:
				x1 = 5 + (7 % 2)
				   = 5 + 1
				   = 6

				x2 = (5 + 7) % 2
				   = 12 % 2
				   = 0

				괄호 위치에 따라 결과가 달라진다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] 곱셈이 덧셈보다 먼저 계산됨" << std::endl;
			std::cout << "============================================" << std::endl;

			int a = 2 + 3 * 4;
			int b = (2 + 3) * 4;

			std::cout << "2 + 3 * 4 = " << a << std::endl;
			std::cout << "(2 + 3) * 4 = " << b << std::endl;
			std::cout << std::endl;

			/*
				설명:
				곱셈(*)은 덧셈(+)보다 우선순위가 높다.

				따라서
					2 + 3 * 4
				는
					2 + 12
				가 되어 14

				반면
					(2 + 3) * 4
				는
					5 * 4
				가 되어 20
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 4] 비교 연산과 산술 연산" << std::endl;
			std::cout << "============================================" << std::endl;

			bool result1 = 2 + 3 > 4;
			bool result2 = 2 + (3 > 4);

			std::cout << "2 + 3 > 4 = " << result1 << std::endl;
			std::cout << "2 + (3 > 4) = " << result2 << std::endl;
			std::cout << std::endl;

			/*
				설명:
				result1:
					2 + 3 > 4
					= 5 > 4
					= true

				result2:
					3 > 4 는 false -> 0
					2 + 0 = 2
					bool 로 저장되므로 true

				즉 괄호 위치에 따라 의미가 달라진다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 5] 같은 우선순위에서는 결합 방향 확인" << std::endl;
			std::cout << "============================================" << std::endl;

			int x = 20 - 5 - 3;
			int y = 20 / 2 / 2;

			std::cout << "20 - 5 - 3 = " << x << std::endl;
			std::cout << "20 / 2 / 2 = " << y << std::endl;
			std::cout << std::endl;

			/*
				설명:
				- 와 / 는 같은 우선순위에서 보통 왼쪽에서 오른쪽으로 계산된다.

				20 - 5 - 3
				= (20 - 5) - 3
				= 15 - 3
				= 12

				20 / 2 / 2
				= (20 / 2) / 2
				= 10 / 2
				= 5
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 6] 대입은 오른쪽에서 왼쪽으로" << std::endl;
			std::cout << "============================================" << std::endl;

			int a, b, c;
			a = b = c = 5;

			std::cout << "a = " << a << std::endl;
			std::cout << "b = " << b << std::endl;
			std::cout << "c = " << c << std::endl;
			std::cout << std::endl;

			/*
				설명:
				대입 연산자는 오른쪽에서 왼쪽으로 처리된다.

				c = 5
				b = c
				a = b

				결과적으로 모두 5가 된다.
			*/
		}

		/*
			실무 팁:
			연산자 우선순위를 전부 외우려 하기보다
			헷갈릴 수 있는 부분은 괄호로 명확하게 표현하는 것이 좋다.

			예:
				int x = (a + b) * c;
				if ((hp > 0) && (mp > 10))
		*/

		system("pause");
	}

	void Test()
	{
		//assignment_operator();

		//arithmetic_operators();

		//compound_assignment();

		//increment_n_decrement();

		//relational_n_comparison_operators();

		//logical_operators();

		//conditional_ternary_operator();

		//comma_operator();

		//bitwise_operators();

		//explicit_type_casting_operator();

		//sizeof_operator();

		//other_operators();

		//precedence_of_operators();
	}

}// end of Operators