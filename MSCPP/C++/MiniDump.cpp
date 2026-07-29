#include "stdafx.h"

#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")

namespace MiniDump
{
	LONG WINAPI writeDump(EXCEPTION_POINTERS* exceptionInfo)
	{
		HANDLE file = CreateFile(
			_T("crash.dmp"),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (file == INVALID_HANDLE_VALUE)
			return EXCEPTION_CONTINUE_SEARCH;

		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ExceptionPointers = exceptionInfo;
		dumpInfo.ClientPointers = FALSE;

		BOOL result = MiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			file,
			MiniDumpWithFullMemory,
			&dumpInfo,
			NULL,
			NULL
		);

		CloseHandle(file);

		return result ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
	}

	void causeCrash()
	{
		int* p = nullptr;
		*p = 123;
	}

	void mini_dump_what()
	{
		/*
			📚 C++ Crash Dump 생성 예제

			  1. 개요
				- 프로그램에서 처리되지 않은 예외가 발생했을 때
				  MiniDumpWriteDump API를 사용하여 dump 파일을 생성하는 예제
				- 사용 가능한 환경:
					* Visual Studio 2015 이상
					* Windows 환경
					* Dbghelp.lib 필요
				- 프로그램 비정상 종료 원인 분석, 크래시 재현,
				  운영 환경 장애 분석 등에 사용

			  2. 기본 개념
				- SetUnhandledExceptionFilter:
					처리되지 않은 예외 발생 시 호출될 예외 처리 함수를 등록
				- EXCEPTION_POINTERS:
					예외 발생 당시의 레지스터, 스택, 예외 코드 정보를 포함
				- MINIDUMP_EXCEPTION_INFORMATION:
					덤프 생성 시 현재 스레드 ID와 예외 정보를 전달하는 구조체
				- MiniDumpWriteDump:
					현재 프로세스 상태를 .dmp 파일로 저장하는 Windows API

			  3. 핵심 특징
				- 프로그램이 크래시되는 순간의 상태를 dump 파일로 저장
				- PDB 파일이 함께 있으면 Visual Studio에서 콜스택과 소스 위치 확인 가능
				- MiniDumpNormal, MiniDumpWithFullMemory 등 옵션에 따라
				  저장되는 정보의 범위가 달라짐

			  4. 실행 흐름
				- main()
				  → SetUnhandledExceptionFilter()로 dump handler 등록
				  → CauseCrash() 호출
				  → nullptr 접근으로 예외 발생
				  → WriteDump() 호출
				  → crash.dmp 파일 생성
				  → 프로그램 종료

			  5. 대표 메서드 또는 주요 코드
				- WriteDump():
					예외 정보를 받아 dump 파일을 생성하는 핵심 함수
				- CreateFile():
					dump 파일을 생성하거나 기존 파일을 덮어씀
				- MiniDumpWriteDump():
					프로세스 상태를 dump 파일로 기록
				- CauseCrash():
					테스트용 크래시를 의도적으로 발생시키는 함수

			  6. 멀티 스레드 환경에서 작동 특징
				- SetUnhandledExceptionFilter는 프로세스 단위로 등록됨
				- 어느 스레드에서 처리되지 않은 예외가 발생해도 handler가 호출될 수 있음
				- dumpInfo.ThreadId에는 예외가 발생한 스레드 ID를 저장
				- MiniDumpWithFullMemory 옵션 사용 시 다른 스레드의 스택과 메모리도
				  분석에 도움이 될 수 있음

			  7. 주의점
				- Dbghelp.dll 버전에 따라 MiniDumpWriteDump 동작이 다를 수 있음
				- 예외 handler 내부에서는 복잡한 로직, 동적 메모리 할당,
				  로그 시스템 호출 등을 최소화하는 것이 안전
				- dump 파일과 동일한 빌드의 exe, pdb 파일이 있어야 정확한 분석 가능
				- Release 빌드에서는 최적화로 인해 콜스택이나 변수 정보가 제한될 수 있음
				- dump 파일에는 민감한 메모리 정보가 포함될 수 있으므로 보안 관리 필요

			  8. 예상 결과
				- 프로그램 실행 후 nullptr 접근으로 크래시 발생
				- 실행 파일 위치에 crash.dmp 파일 생성
				- Visual Studio에서 crash.dmp를 열면 예외 발생 위치와 콜스택 확인 가능
		*/
		{
			SetUnhandledExceptionFilter(writeDump);

			std::cout << "C++ Dump Test Start" << std::endl;
		}

		system("pause");
	}


	void Test()
	{
		mini_dump_what();
	}

}// end of MiniDump