#pragma once 

///////////////////////////////////////////////////////////////////////////////
/// @file CRT_MemoryCheck.h
/// @title CRT Dubug 활성화 코드
/// @brief CRT Dubug 를 활성화 할 경우 MemoryCheck.h 파일을 include 해줘라 !!!
/// 
/// @author justin
/// @date 4-3-2004 11:45:20
///////////////////////////////////////////////////////////////////////////////

#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#define Assert(f)				_ASSERT(f)
#define IS_VALID_PTR(p)			_CrtIsValidPointer(p, sizeof(p), true) 
#define Assert_Valid_Ptr(p)		Assert(IS_VALID_PTR(p)) 

#endif //_CRTDBG_MAP_ALLOC

///////////////////////////////////////////////////////////////////////////////
/// @file CRT_MemoryCheck.h
/// @title CRT Debug Flags
/// @brief 메모리 누수 체크 !!! 개발 초기시 반드시 체크 요망
///
///		메모리 누수 체크 출력시 출력창
///		c:\Sample\debug\main.cpp(25) : {44} client block at 0x003710B0, subtype 0, 256 bytes long. 
///		Data: <string2 > 73 74 72 69 6E 67 32 00 CD CD CD CD CD CD CD CD 
///		Object dump complete.
///
///     의미: main.cpp 파일에서 25 라인 new 한 곳에서 256 바이트 해제 안됨...
///
///     _CRTDBG_LEAK_CHECK_DF : 프로그램 종료시 메모리 릭 상태 출력
///     _CRTDBG_CHECK_ALWAYS_DF : 모든 메모리 관련 연산시 _CrtCheckMemory - 비용 부담 많음 !!!
/// 
/// @author justin
/// @date 4-3-2004 11:51:43
///////////////////////////////////////////////////////////////////////////////

// 해당 new 라인 출력용
#ifdef _DEBUG
	#define new new(_CLIENT_BLOCK, __FILE__, __LINE__) // debug 용 new 정의
#endif 

// CRT 메모리 체크 함수
#define SET_CRT_MEMORY_CHECK(a) _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))


#define CRT_MEMORY_LEAK_ALWAYS_CHECK_FLAGS (_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF)
#define CRT_MEMORY_ALWAYS_CHECK_FLAGS (_CRTDBG_CHECK_ALWAYS_DF)
