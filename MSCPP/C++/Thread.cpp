#include "stdafx.h"

#include <process.h>
#include <Windows.h>

namespace Thread
{
	/*
		Thread

		When a thread is created, the code, data, and heap data of the process are shared,
		and only the stack area of ​​the thread is created separately.
		
		When creating a thread using a function such as CreateThread(),
		you can pass the thread's stack size as an argument.
		If you pass it as 0, 1 MB is allocated by default.

		But what happens if you take less than one megabyte of this stack size?
		The bottom line is that even if you explicitly set the stack size to be less than 1 mb,
		the OS will fix it at 1 mb.
		That is, the minimum stack size of a thread is 1 mb.

		You should not use the CreateThread() function when you want to use Thread !!!.
		This is because if you use the c/c++ runtime function inside a thread statement created with CreateThread,
		the c/c++ function will not execute and only the thread will die.
	*/

	UINT32 WINAPI ThreadFunc(void* param)
	{
		int MaxCount = (int)param;

		// child thread, create thread by beginthreadex() function
		for (int i = 1; i <= MaxCount; ++i) {
			Sleep(1000);
			printf("Counted : %d !!! - ThreadID:%d\n"
				  , i, GetCurrentThreadId());
		}

		_endthreadex(0); // local memory clear !!!

		return 0;
	}

	void c_run_time_thread()
	{
		int MaxCount = 1000;

		HANDLE hThread;
		UINT32 threadID;

		// created thread
		hThread = (HANDLE)_beginthreadex( NULL
			                            , 0
			                            , ThreadFunc
			                            , (void*)&MaxCount
			                            , 0
			                            , (UINT32*)&threadID);
		if (hThread == 0)
		{
			printf("Failed to create Thread !!!\n");
			return;
		}
		printf("created Thread Handle - %d\n", (int)hThread);
		printf("created Thread ID - %d\n", threadID);

		// Wait until second thread terminates.
		// If you comment out the line below,
		// Counter will not be correct because the thread has not terminated,
		// and counter most likely has not been incremented to 1000 yet.  
		::WaitForSingleObject(hThread, INFINITE);

		// destroy the thread object.  
		CloseHandle(hThread);

		system("pause");
		/*
		output:
			created Thread Handle - 56
			created Thread ID - 56
			Counted : 1 !!! - ThreadID:55336
			...
		*/
	}

	void Test()
	{
		//c_run_time_thread();
	}
}