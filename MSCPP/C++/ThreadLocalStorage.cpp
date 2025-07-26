#include "stdafx.h"

#include <process.h>
#include <Windows.h>

namespace ThreadLocalStorage
{
	/*
		TLS (Thread Local Storage)
		
		Thread Local Storage (TLS) is the method by which each thread in a given multithreaded process
		can allocate locations in which to store thread-specific data.
		Dynamically bound (run-time) thread-specific data is supported by
		way of the TLS API (TlsAlloc, TlsGetValue, TlsSetValue, and TlsFree).
		For more information about how thread local storage is implemented on Windows,
		see Thread Local Storage (Windows).
		Win32 and the Visual C++ compiler now support statically bound (load-time)
		per-thread data in addition to the existing API implementation.

		In the case of a thread specification error

		 *	__declspec( thread )void func();	// This will generate an error.
		 *	void func1()
			{  
				__declspec( thread )int tls_i;	// This will generate an error.  
			}
		 *	int func2(__declspec( thread )int tls_i )	// This will generate an error.  
			{  
				return tls_i;
			}
		 *	#define Thread  __declspec( thread )  
			extern int tls_i;				// This will generate an error,   
			int __declspec( thread )tls_i;	// since the declaration and definition differ.  
		 *	char __declspec( thread ) *ch;	// Error
		 *	__declspec( thread ) class B
			{  
				// Code
			} BObject;	// OK--BObject is declared thread local.

			class B
			{
				// Code
			};
			__declspec( thread ) B BObject;	// OK--BObject is declared thread local.
		 *	__declspec( thread )int tls_i;
			int *p = &tls_i;	// This will generate an error in C.
		 *	__declspec( thread )int tls_i = tls_i;	// Error in C and C++
			int j = j;	// OK in C++, error in C
			__declspec( thread )int tls_i = sizeof( tls_i )	// Legal in C and C++
	*/

	#define THREAD_COUNT 4 

	__declspec(thread) int tls_i = 1;

	UINT32 WINAPI StaticTLSThreadFunc(void* param)
	{
		tls_i = (int)param;
		printf("Load static TLS value - Value:%d, ThreadID:%d\n"
			  , tls_i, GetCurrentThreadId());

		// child thread, create thread by beginthreadex() function
		for (int i = 1; i <= tls_i; ++i) {
			Sleep(1000);
			printf("Counted : %d !!! - ThreadID:%d\n"
				  , i, GetCurrentThreadId());
		}

		_endthreadex(0); // local memory clear !!!

		return 0;
	}

	void static_tls()
	{
		DWORD IDThread;
		HANDLE hThread[THREAD_COUNT];

		// Create multiple threads. 
		for (int i = 0; i < THREAD_COUNT; i++) {

			uintptr_t arg = tls_i * 5 * (i + 1);

			// created thread
			hThread[i] = (HANDLE)_beginthreadex( NULL
											   , 0
											   , StaticTLSThreadFunc
											   , (void*)arg
											   , 0
											   , (UINT32*)&IDThread);
			// Check the return value for success. 
			if (hThread[i] == 0) {
				printf("Failed to create Thread !!!\n");
				return;
			}
		}

		for (int i = 0; i < THREAD_COUNT; i++) {
			::WaitForSingleObject(hThread[i], INFINITE);
		}

		system("pause");

		/*
		output:
			Load static TLS value - Value:5, Thread:61048
			Load static TLS value - Value:10, Thread:740
			Load static TLS value - Value:20, Thread:49936
			Load static TLS value - Value:15, Thread:53944
			Counted : 1 !!! - ThreadID:61048
			Counted : 1 !!! - ThreadID:740
			Counted : 1 !!! - ThreadID:49936
			Counted : 1 !!! - ThreadID:53944
			Counted : 2 !!! - ThreadID:61048
			Counted : 2 !!! - ThreadID:740
			Counted : 2 !!! - ThreadID:53944
			Counted : 2 !!! - ThreadID:49936
			...
		*/
	}

	DWORD dwTlsIndex;

	void ErrorExit(LPSTR lpszMessage)
	{
		fprintf(stderr, "%s\n", lpszMessage);
		ExitProcess(0);
	}

	void CommonFunc(void)
	{
		void* lpvData;

		// retrieve a data pointer for the current thread.
		lpvData = TlsGetValue(dwTlsIndex);
		if ((lpvData == 0) && (GetLastError() != ERROR_SUCCESS)) {
			ErrorExit("TlsGetValue error");
		}

		// Use the data stored for the current thread. 
		printf( "common: thread %d: lpvData=%lx\n"
			  , GetCurrentThreadId(), (long)lpvData );

		Sleep(5000);
	}

	UINT32 WINAPI DynamicTLSThreadFunc(void* param)
	{
		void* lpvData;

		// Initialize the TLS index for this thread. 
		lpvData = (void*)LocalAlloc(LPTR, 256);
		if (!TlsSetValue(dwTlsIndex, lpvData)) {
			ErrorExit("TlsSetValue error");
		}

		printf("thread %d: lpvData=%lx\n", GetCurrentThreadId(), (long)lpvData);

		CommonFunc();

		// Release the dynamic memory before the thread returns. 
		lpvData = TlsGetValue(dwTlsIndex);
		if (lpvData != 0) {
			LocalFree((HLOCAL)lpvData);
		}

		return 0;
	}

	void dynamic_tls()
	{
		DWORD IDThread;
		HANDLE hThread[THREAD_COUNT];

		// Allocate a TLS index. 
		if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES) {
			ErrorExit("TlsAlloc failed");
		}

		// Create multiple threads. 
		for (int i = 0; i < THREAD_COUNT; i++) {

			// created thread
			hThread[i] = (HANDLE)_beginthreadex( NULL
											   , 0
											   , DynamicTLSThreadFunc
											   , NULL
											   , 0
											   , (UINT32*)&IDThread);
			// Check the return value for success. 
			if (hThread[i] == 0) {
				printf("Failed to create Thread !!!\n");
				return;
			}
		}

		for (int i = 0; i < THREAD_COUNT; i++) {
			::WaitForSingleObject(hThread[i], INFINITE);
		}

		TlsFree(dwTlsIndex);

		system("pause");

		/*
		output:
			thread 60092: lpvData=304df0
			common: thread 60092: lpvData=304df0
			thread 57708: lpvData=304710
			common: thread 57708: lpvData=304710
			thread 56372: lpvData=305110
			common: thread 56372: lpvData=305110
			thread 56964: lpvData=304600
			common: thread 56964: lpvData=304600
		*/
	}


	void Test()
	{
		//dynamic_tls();

		//static_tls();
	}
}