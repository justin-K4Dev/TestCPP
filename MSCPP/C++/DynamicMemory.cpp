#include "stdafx.h"

#include <windows.h>  
#include <malloc.h>  


namespace DynamicMemory
{
	const int constval = 30;    // 상수

	int uninitial;  // 초기화되지 않은 전역변수
	int initial = 30;   // 초기화된 전역변수
	static int staticval = 70;  // 정적변수

	int function() {    // 함수
		return 20;
	}

	void memory_structure()
	{
		/*
			Low address

			[Text]

			텍스트 영역은 아주 쉽게 말하면 코드를 실행하기 위해 저장되어있는 영역입니다.
			흔히 코드 영역이라고도 하는데, 프로그램을 실행시키기 위해 구성되는 것들이 저장되는 영역입니다.
			한마디로 명령문들이 저장되는 것인데, 제어문, 함수, 상수들이 이 영역에 저장됩니다.


			[Data]

			데이터 영역은 우리가 작성한 코드에서 전역변수, 정적변수 등이 저장되는 공간입니다.
			이들의 특징을 보면 보통 메인(main)함수 전(프로그램 실행 전)에 선언되어 프로그램이 끝날 때 까지 메모리에 남아있는 변수들이라는 특징이 있습니다.
			좀 더 구체적으로 말하자면 Data영역도 크게 두 가지로 나뉩니다.
			초기화 된 변수 영역(initialized data segment)과 초기화되지 않은 변수 영역(uninitialized data segment)으로 나뉘죠.
			그리고 그 중 초기화되지 않은 변수 영역은 BSS(Block Started by Symbol) 이라고도 합니다.


			[Heap]

			힙 영역은 쉽게 말해서 '사용자에 의해 관리되는 영역'입니다. 흔히 동적으로 할당 할 변수들이 여기에 저장된다고 보시면 됩니다.
			또한 Java나 C++에서 new 연산자로 생성하는 경우 또는 class, 참조 변수들도 Heap영역에 차지하게 됩니다.
			다만, 이는 언어마다 조금씩 상이하니 일단은 '동적 할당 영역'이라고 알아두시면 될 것 같습니다.
			그리고 Heap 영역은 대개 '낮은 주소에서 높은 주소로 할당(적재)됩니다'


			[Stack]

			스택 영역은 함수를 호출 할 때 지역변수, 매개변수들이 저장되는 공간입니다.
			메인(main) 함수안에서의 변수들도 당연 이에 포함되죠.
			그리고 함수가 종료되면 해당 함수에 할당된 변수들을 메모리에서 해제시킵니다.
			한마디로 Stack 자료구조의 pop과 같은 기능이죠.

			여러분이 함수를 '재귀' 호출 할 때, 재귀가 깊어져 Stack Overflow 를 경험해보셨을 겁니다.
			이 이유가 재귀를 반복적으로 호출하면서 Stack 메모리 영역에 해당 함수의 지역변수, 매개변수들이 계속 할당되다가
			OS에서 할당해준 Stack영역의 메모리 영역을 넘어버리면 발생하는 오류입니다.
			Stack영역은 Heap영역과 반대로 높은주소에서 낮은주소로 메모리에 할당됩니다.


			High address
		*/

		int localval1 = 30;   // 지역변수 1
		int localval2;      // 지역변수 2

		printf("숫자 입력 : ");
		scanf("%d", &localval2);

		char *arr = (char*)malloc(sizeof(char) * 10);  // 동적 할당 변수

		/* 포인터 출력 영역 */
		printf("상수 Memory Address : \t\t %p \n", &constval);
		printf("비초기화 변수 Memory Address :\t %p \n", &uninitial);
		printf("초기화 변수 Memory Address : \t %p \n", &initial);
		printf("정적 변수 Memory Address : \t %p \n", &staticval);
		printf("함수 Memory Address : \t\t %p \n", function);
		printf("지역변수1 Memory Address : \t %p \n", &localval1);
		printf("지역변수2 Memory Address : \t %p \n", &localval2);
		printf("동적할당변수 Memory Address : \t %p \n\n", arr);

		system("pause");
	}

	void dynamic_memory()
	{
		/*
			In the programs seen in previous chapters,
			all memory needs were determined before program execution by defining the variables needed.
			But there may be cases where the memory needs of a program can only be determined during runtime.
			For example, when the memory needed depends on user input.
			On these cases, programs need to dynamically allocate memory,
			for which the C++ language integrates the operators new and delete.
		*/
	}

	void operators_new_and_new_array()
	{
		/*
			Dynamic memory is allocated using operator new. new is followed by a data type specifier and,
			if a sequence of more than one element is required, the number of these within brackets [].
			It returns a pointer to the beginning of the new block of memory allocated.
			Its syntax is: 

				pointer = new type
				pointer = new type [number_of_elements]

			The first expression is used to allocate memory to contain one single element of type type.
			The second one is used to allocate a block (an array) of elements of type type,
			where number_of_elements is an integer value representing the amount of these.
			For example:

				int * foo;
				foo = new int [5];

			In this case, the system dynamically allocates space for five elements of type int
			and returns a pointer to the first element of the sequence, which is assigned to foo (a pointer).
			Therefore, foo now points to a valid block of memory with space for five elements of type int.

				foo [int][  ][  ][  ][  ]

			Here, foo is a pointer, and thus, the first element pointed to by foo can be accessed either
			with the expression foo[0] or the expression *foo (both are equivalent).
			The second element can be accessed either with foo[1] or *(foo+1), and so on...

			There is a substantial difference between declaring a normal array
			and allocating dynamic memory for a block of memory using new.
			The most important difference is that the size of a regular array needs to be a constant expression,
			and thus its size has to be determined at the moment of designing the program,
			before it is run, whereas the dynamic memory allocation performed by new allows to assign memory
			during runtime using any variable value as size.

			The dynamic memory requested by our program is allocated by the system from the memory heap.
			However, computer memory is a limited resource, and it can be exhausted.
			Therefore, there are no guarantees that all requests to allocate memory
			using operator new are going to be granted by the system.

			C++ provides two standard mechanisms to check if the allocation was successful:

			One is by handling exceptions. Using this method,
			an exception of type bad_alloc is thrown when the allocation fails.
			Exceptions are a powerful C++ feature explained later in these tutorials.
			But for now, you should know that if this exception is thrown
			and it is not handled by a specific handler, the program execution is terminated.

			This exception method is the method used by default by new, and is the one used in a declaration like:

				foo = new int [5];  // if allocation fails, an exception is thrown

			The other method is known as nothrow, and what happens when it is used is
			that when a memory allocation fails, instead of throwing a bad_alloc exception
			or terminating the program, the pointer returned by new is a null pointer,
			and the program continues its execution normally.

			This method can be specified by using a special object called nothrow, declared in header <new>,
			as argument for new:

				foo = new (nothrow) int [5];

			In this case, if the allocation of this block of memory fails,
			the failure can be detected by checking if foo is a null pointer:

				int * foo;
				foo = new (nothrow) int [5];
				if (foo == nullptr) {
					// error assigning memory. Take measures.
				}

			This nothrow method is likely to produce less efficient code than exceptions,
			since it implies explicitly checking the pointer value returned after each and every allocation.
			Therefore, the exception mechanism is generally preferred, at least for critical allocations.
			Still, most of the coming examples will use the nothrow mechanism due to its simplicity.
		*/
	}

	void operators_delete_and_delete_array()
	{
		/*
			In most cases, memory allocated dynamically is only needed during specific periods of time within a program;
			once it is no longer needed, it can be freed so that the memory becomes available again for other requests of dynamic memory.
			This is the purpose of operator delete, whose syntax is:

				delete pointer;
				delete[] pointer;

			The first statement releases the memory of a single element allocated using new,
			and the second one releases the memory allocated for arrays of elements using new
			and a size in brackets ([]).

			The value passed as argument to delete shall be either a pointer to a memory block previously allocated
			with new, or a null pointer (in the case of a null pointer, delete produces no effect).
		*/
		{
			int i, n;
			int * p;

			std::cout << "How many numbers would you like to type? ";
			std::cin >> i;

			p = new int[i];
			if (p == nullptr)
				std::cout << "Error: memory could not be allocated";
			else
			{
				for (n = 0; n < i; n++)
				{
					std::cout << "Enter number: ";
					std::cin >> p[n];
				}
				std::cout << "You have entered: ";
				for (n = 0; n < i; n++)
					std::cout << p[n] << ", ";

				delete[] p;
			}

			system("pause");

			/*
			output:
				How many numbers would you like to type? 5
				Enter number : 75
				Enter number : 436
				Enter number : 1067
				Enter number : 8
				Enter number : 32
				You have entered: 75, 436, 1067, 8, 32,
			*/
		}
		/*
			Notice how the value within brackets in the new statement is a variable value entered by the user (i),
			not a constant expression:

				p = new (nothrow) int[i];

			There always exists the possibility that the user introduces a value for i so big
			that the system cannot allocate enough memory for it.
			For example, when I tried to give a value of 1 billion to the "How many numbers" question,
			my system could not allocate that much memory for the program,
			and I got the text message we prepared for this case (Error: memory could not be allocated).

			It is considered good practice for programs to always be able to handle failures to allocate memory,
			either by checking the pointer value (if nothrow) or by catching the proper exception.
		*/
	}

	void dynamic_memory_in_c()
	{
		/*
			C++ integrates the operators new and delete for allocating dynamic memory.
			But these were not available in the C language; instead, it used a library solution,
			with the functions malloc, calloc, realloc and free, defined in the header <cstdlib> (known as <stdlib.h> in C).
			
			The functions are also available in C++ and can also be used to allocate and deallocate dynamic memory.

			Note, though, that the memory blocks allocated by these functions are not necessarily compatible with those returned by new,
			so they should not be mixed; each one should be handled with its own set of functions or operators.
		*/
		{
			system("pause");
		}
	}

	void allocates_size_bytes_from_the_program_stack()
	{
		/*
			_alloca allocates size bytes from the program stack.
			The allocated space is automatically freed when the calling function exits
			(not when the allocation merely passes out of scope).
			Therefore, do not pass the pointer value returned by _alloca as an argument to free.
		*/
		{
			int     size = 1000;
			int     errcode = 0;
			void    *pData = NULL;

			// Note: Do not use try/catch for _alloca,  
			// use __try/__except, since _alloca throws  
			// Structured Exceptions, not C++ exceptions.  

			__try {
				// An unbounded _alloca can easily result in a   
				// stack overflow.  
				// Checking for a size < 1024 bytes is recommended.  
				if (size > 0 && size < 1024) {
					pData = _alloca(size);
					printf_s("Allocated %d bytes of stack at 0x%p", size, pData);
				}
				else {
					printf_s("Tried to allocate too many bytes.\n");
				}
			}

			// If an exception occured with the _alloca function  
			__except (GetExceptionCode() == STATUS_STACK_OVERFLOW) {
				printf_s("_alloca failed!\n");

				// If the stack overflows, use this function to restore.  
				errcode = _resetstkoflw();
				if (errcode) {
					printf_s("Could not reset the stack!\n");
					_exit(1);
				}
			};

			system("pause");

			/*
			output:
				Allocated %d bytes of stack at 0x000000000020F4A0
			*/
		}
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