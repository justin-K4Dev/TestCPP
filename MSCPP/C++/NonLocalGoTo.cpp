#include "stdafx.h"

#include <cstdio>
#include <vector>
#include <cstdarg>
#include <cstring>
#include <csetjmp>


namespace NonLocalGoTo
{
	/*
		Non-local GoTo

		longjmp() restores the environment saved by the last call of setjmp().
		It then returns in such a way that execution continues as if
		the call of setjmp() had just returned the value val to the corresponding call to setjmp(),
		which must not itself have returned in the interim.
		longjmp() cannot return the value 0.
		If longjmp() is invoked with it's second argument 0, it will return a 1.
		All accessible data have values as of the time longjmp() was called.
	*/

	int call_count = 0;
	std::jmp_buf jumper;

	void jump_func()
	{
		call_count++;

		if (call_count < 10) {
			std::longjmp(jumper, 1); // jump setjmp() function !!!
		}
		else {
			std::longjmp(jumper, 2); // jump setjmp() function !!!
		}
	}

	void non_local_goto_use()
	{
		bool flag = false;

		int result = setjmp(jumper); // set jump position

		if (result == 2) {
			flag = true;
		}
		
		if (flag != true) {
			jump_func();
		}

		printf("count=%d, flag=%d\n", call_count, flag);

		system("pause");

		/*
		output:
			count=10, flag=1
		*/
	}

	void Test()
	{
		//non_local_goto_use();
	}
}