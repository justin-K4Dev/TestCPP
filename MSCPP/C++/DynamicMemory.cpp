#include "stdafx.h"

#include <windows.h>  
 

namespace DynamicMemory
{
	const int constval = 30;      // 상수
	int uninitial;                // 초기화되지 않은 전역 변수 (BSS)
	int initial = 30;             // 초기화된 전역 변수 (Data)
	static int staticval = 70;    // 정적 전역 변수 (Data)

	int function()
	{
		return 20;
	}

	void memory_structure()
	{
		/*
			📚 메모리 구조 (Memory Structure)

			프로그램이 실행될 때 메모리는 대략 다음과 같은 영역으로 나뉜다.

			낮은 주소
			-----------------------------------
			[Text / Code]
			[Data / BSS]
			[Heap]
				  ...
			[Stack]
			-----------------------------------
			높은 주소

			1) Text(Code) 영역
			   - 실행할 기계어 코드
			   - 함수 코드
			   - 읽기 전용 상수 일부
			   - 프로그램 명령문

			2) Data 영역
			   - 초기화된 전역 변수
			   - 초기화된 static 변수

			3) BSS 영역
			   - 초기화되지 않은 전역 변수
			   - 초기화되지 않은 static 변수

			4) Heap 영역
			   - 사용자가 동적으로 할당하는 메모리
			   - new / malloc 등으로 확보
			   - 보통 낮은 주소 -> 높은 주소 방향으로 확장

			5) Stack 영역
			   - 지역 변수
			   - 함수 매개변수
			   - 함수 호출 정보
			   - 보통 높은 주소 -> 낮은 주소 방향으로 확장

			중요:
			운영체제 / 컴파일러 / 빌드 옵션에 따라 실제 배치는 달라질 수 있다.
			아래 주소 출력은 "개념 이해용"이다.
		*/

		int localval1 = 30;   // 지역 변수
		int localval2 = 0;    // 지역 변수

		printf("숫자 입력 : ");
		scanf("%d", &localval2);

		char* arr = (char*)malloc(sizeof(char) * 10);  // 힙 영역 동적 할당

		printf("상수 Memory Address : \t\t %p \n", &constval);
		printf("비초기화 변수 Memory Address :\t %p \n", &uninitial);
		printf("초기화 변수 Memory Address : \t %p \n", &initial);
		printf("정적 변수 Memory Address : \t %p \n", &staticval);
		printf("함수 Memory Address : \t\t %p \n", function);
		printf("지역변수1 Memory Address : \t %p \n", &localval1);
		printf("지역변수2 Memory Address : \t %p \n", &localval2);
		printf("동적할당변수 Memory Address : \t %p \n\n", arr);

		free(arr);
		arr = nullptr;

		/*
			설명:
			- constval, initial, staticval 은 전역/정적 성격이라 Data 계열 영역에 놓일 가능성이 크다
			- uninitial 은 BSS 계열에 놓일 가능성이 크다
			- function 은 코드(Text) 영역 주소를 가진다
			- localval1, localval2 는 Stack 영역
			- arr 이 가리키는 실제 메모리는 Heap 영역

			또한 보통:
			- Heap 은 아래에서 위로 커지고
			- Stack 은 위에서 아래로 커진다
		*/

		system("pause");
	}

	void dynamic_memory()
	{
		/*
			📚 동적 메모리 (Dynamic memory)

			이전 예제들에서는 필요한 메모리 크기가
			대부분 프로그램 작성 시점에 이미 결정되어 있었다.

			예:
				int arr[10];

			하지만 어떤 경우에는
			실행 중(runtime)에야 필요한 메모리 크기를 알 수 있다.

			예:
			- 사용자 입력에 따라 배열 크기가 달라지는 경우
			- 파일 크기에 따라 버퍼 크기가 달라지는 경우
			- 객체 수가 실행 도중 결정되는 경우

			이럴 때 사용하는 것이 동적 메모리 할당이다.

			C++에서는 이를 위해
				new / delete
			연산자를 제공한다.
		*/

		{
			std::cout << "동적 메모리는 실행 중 필요한 크기를 결정할 수 있다." << std::endl;
			std::cout << "대표적으로 new / delete 를 사용한다." << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void operators_new_and_new_array()
	{
		/*
			📚 new 와 new[] 연산자

			C++에서 동적 메모리는 new 로 할당한다.

			문법:
				포인터 = new 타입;
				포인터 = new 타입[개수];

			예:
				int* p1 = new int;
				int* p2 = new int[5];

			의미:
			- new int       -> int 1개를 힙에 할당
			- new int[5]    -> int 5개 배열을 힙에 할당

			반환값:
			- 새로 할당된 메모리의 시작 주소
			- 즉 포인터
		*/

		{
			int* p1 = new int;
			*p1 = 123;

			std::cout << "*p1 = " << *p1 << std::endl;
			std::cout << "p1  = " << p1 << std::endl;

			delete p1;
			p1 = nullptr;

			std::cout << std::endl;

			/*
				설명:
				new int 는 int 하나를 힙에 만들고
				그 주소를 p1 에 저장한다.
			*/
		}

		{
			int* foo = new int[5];

			for (int i = 0; i < 5; ++i)
			{
				foo[i] = (i + 1) * 10;
			}

			for (int i = 0; i < 5; ++i)
			{
				std::cout << "foo[" << i << "] = " << foo[i] << std::endl;
			}

			delete[] foo;
			foo = nullptr;

			std::cout << std::endl;

			/*
				설명:
				new int[5] 는 int 배열 5개를 동적 할당한다.
				배열 형태로 할당했으므로 해제도 delete[] 를 써야 한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 3] new 와 일반 배열의 차이" << std::endl;
			std::cout << "============================================" << std::endl;

			int size;
			std::cout << "원하는 배열 크기 입력: ";
			std::cin >> size;

			int* dynamicArr = new int[size];

			for (int i = 0; i < size; ++i)
			{
				dynamicArr[i] = i + 1;
			}

			for (int i = 0; i < size; ++i)
			{
				std::cout << dynamicArr[i] << ' ';
			}
			std::cout << std::endl << std::endl;

			delete[] dynamicArr;
			dynamicArr = nullptr;

			/*
				설명:
				일반 배열은 크기가 보통 컴파일 시점에 정해져야 하지만,
				new 를 사용하면 실행 중 입력값으로 크기를 정할 수 있다.
			*/
		}

		/*
			추가 설명:
			new 는 메모리 할당 실패 시 기본적으로 예외를 던질 수 있다.
			또는 nothrow 버전을 쓰면 nullptr 반환으로 확인할 수도 있다.

				int* p = new (std::nothrow) int[5];
				if (p == nullptr)
				{
					// 실패 처리
				}
		*/

		system("pause");
	}

	void operators_delete_and_delete_array()
	{
		/*
			📚 delete 와 delete[] 연산자

			new 로 할당한 메모리는
			더 이상 필요 없을 때 반드시 해제해야 한다.

			문법:
				delete 포인터;
				delete[] 포인터;

			규칙:
			- new 로 할당한 단일 객체      -> delete
			- new[] 로 할당한 배열        -> delete[]

			이 규칙을 맞추지 않으면
			정의되지 않은 동작(Undefined Behavior)이 발생할 수 있다.
		*/

		{
			int i, n;
			int* p;

			std::cout << "How many numbers would you like to type? ";
			std::cin >> i;

			p = new int[i];

			if (p == nullptr)
			{
				std::cout << "Error: memory could not be allocated";
			}
			else
			{
				for (n = 0; n < i; n++)
				{
					std::cout << "Enter number: ";
					std::cin >> p[n];
				}

				std::cout << "You have entered: ";
				for (n = 0; n < i; n++)
				{
					std::cout << p[n] << ", ";
				}
				std::cout << std::endl;

				delete[] p;
				p = nullptr;
			}

			std::cout << std::endl;

			/*
				설명:
				p = new int[i];
				로 배열 할당했으므로
				delete[] p;
				로 해제해야 한다.
			*/
		}

		{
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] delete 후 nullptr 대입" << std::endl;
			std::cout << "============================================" << std::endl;

			int* p = new int;
			*p = 999;

			std::cout << "*p = " << *p << std::endl;

			delete p;
			p = nullptr;

			std::cout << "delete 후 p = " << p << std::endl;
			std::cout << std::endl;

			/*
				설명:
				delete 후 포인터에 nullptr 을 넣어두면
				해제된 주소를 실수로 다시 사용하는 위험을 줄일 수 있다.
			*/
		}

		/*
			핵심:
			- new  -> delete
			- new[] -> delete[]

			반드시 짝을 맞춰야 한다.
		*/

		system("pause");
	}

	void dynamic_memory_in_c()
	{
		/*
			📚 C 방식 동적 메모리 (malloc / calloc / realloc / free)

			C++에는 new / delete 가 있지만,
			C 언어에서는 주로 다음 함수들을 사용했다.

				malloc
				calloc
				realloc
				free

			이 함수들은 <cstdlib> 에 정의되어 있다.

			중요:
			C++에서도 사용할 수는 있지만,
			new / delete 와 섞어 쓰면 안 된다.

			즉:
			- malloc 으로 할당 -> free 로 해제
			- new 로 할당     -> delete 로 해제

			서로 혼용하면 안 된다.
		*/

		{
			int count = 5;

			int* arr = (int*)malloc(sizeof(int) * count);
			if (arr == nullptr)
			{
				std::cout << "malloc failed" << std::endl;
			}
			else
			{
				for (int i = 0; i < count; ++i)
				{
					arr[i] = (i + 1) * 100;
				}

				for (int i = 0; i < count; ++i)
				{
					std::cout << arr[i] << ' ';
				}
				std::cout << std::endl;

				free(arr);
				arr = nullptr;
			}

			std::cout << std::endl;

			/*
				설명:
				malloc 은 메모리만 할당하고,
				생성자 같은 C++ 객체 초기화는 하지 않는다.

				기본형(int 등)에는 쓸 수 있지만,
				C++ 객체에는 일반적으로 new 가 더 적절하다.
			*/
		}

		{
			int count = 5;

			int* arr = (int*)calloc(count, sizeof(int));
			if (arr != nullptr)
			{
				std::cout << "calloc result: ";
				for (int i = 0; i < count; ++i)
				{
					std::cout << arr[i] << ' ';
				}
				std::cout << std::endl;

				free(arr);
				arr = nullptr;
			}

			std::cout << std::endl;

			/*
				설명:
				calloc 은 할당과 동시에 0으로 초기화한다.
			*/
		}

		system("pause");
	}

	void allocates_size_bytes_from_the_program_stack()
	{
		/*
			📚 _alloca : 스택에서 동적 크기 메모리 확보

			_alloca 는 힙(heap)이 아니라 스택(stack)에서 메모리를 확보한다.

			특징:
			1) 함수가 끝나면 자동으로 해제된다
			2) free 로 해제하면 안 된다
			3) 너무 크게 할당하면 stack overflow 위험이 있다

			즉 매우 빠를 수는 있지만,
			사용 범위가 제한적이고 주의가 많이 필요하다.

			중요:
			_alloca 로 확보한 메모리는
			"함수 종료 시" 자동 해제된다.
			블록 scope 가 끝난다고 바로 해제되는 것은 아니다.
		*/

		{
			int size = 1000;
			int errcode = 0;
			void* pData = NULL;

			// _alloca 는 C++ 예외가 아니라 Structured Exception 을 사용할 수 있으므로
			// __try / __except 예제가 자주 같이 등장한다.

			__try
			{
				// 너무 큰 _alloca 는 stack overflow 위험이 매우 큼
				if (size > 0 && size < 1024)
				{
					pData = _alloca(size);
					printf_s("Allocated %d bytes of stack at 0x%p\n", size, pData);
				}
				else
				{
					printf_s("Tried to allocate too many bytes.\n");
				}
			}
			__except (GetExceptionCode() == STATUS_STACK_OVERFLOW)
			{
				printf_s("_alloca failed!\n");

				errcode = _resetstkoflw();
				if (errcode == 0)
				{
					printf_s("Could not reset the stack!\n");
					_exit(1);
				}
			}

			/*
				설명:
				이 예제는 Windows/MSVC 전용에 가까운 코드이다.

				_alloca 는 편리하지만:
				- 너무 큰 크기 금지
				- 반환 포인터를 함수 밖으로 넘기면 안 됨
				- free 사용 금지
				같은 제약이 있다.
			*/
		}

		{
			std::cout << std::endl;
			std::cout << "============================================" << std::endl;
			std::cout << "[TEST 2] _alloca 핵심 주의점" << std::endl;
			std::cout << "============================================" << std::endl;

			std::cout << "1. 힙이 아니라 스택에서 할당" << std::endl;
			std::cout << "2. 함수 종료 시 자동 해제" << std::endl;
			std::cout << "3. free 사용 금지" << std::endl;
			std::cout << "4. 큰 크기는 stack overflow 위험" << std::endl;
			std::cout << std::endl;
		}

		system("pause");
	}

	void Test()
	{
		//dynamic_memory();

		//operators_new_and_new_array();

		//operators_delete_and_delete_array();

		//dynamic_memory_in_c();

		//allocates_size_bytes_from_the_program_stack();

		//memory_structure();
	}

}// end of DynamicMemory