#pragma once 

///////////////////////////////////////////////////////////////////////////////
/// @file CRT_MemoryCheck.h
/// @title CRT Dubug Ȱ��ȭ �ڵ�
/// @brief CRT Dubug �� Ȱ��ȭ �� ��� MemoryCheck.h ������ include ����� !!!
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
/// @brief �޸� ���� üũ !!! ���� �ʱ�� �ݵ�� üũ ���
///
///		�޸� ���� üũ ��½� ���â
///		c:\Sample\debug\main.cpp(25) : {44} client block at 0x003710B0, subtype 0, 256 bytes long. 
///		Data: <string2 > 73 74 72 69 6E 67 32 00 CD CD CD CD CD CD CD CD 
///		Object dump complete.
///
///     �ǹ�: main.cpp ���Ͽ��� 25 ���� new �� ������ 256 ����Ʈ ���� �ȵ�...
///
///     _CRTDBG_LEAK_CHECK_DF : ���α׷� ����� �޸� �� ���� ���
///     _CRTDBG_CHECK_ALWAYS_DF : ��� �޸� ���� ����� _CrtCheckMemory - ��� �δ� ���� !!!
/// 
/// @author justin
/// @date 4-3-2004 11:51:43
///////////////////////////////////////////////////////////////////////////////

// �ش� new ���� ��¿�
#ifdef _DEBUG
	#define new new(_CLIENT_BLOCK, __FILE__, __LINE__) // debug �� new ����
#endif 

// CRT �޸� üũ �Լ�
#define SET_CRT_MEMORY_CHECK(a) _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))


#define CRT_MEMORY_LEAK_ALWAYS_CHECK_FLAGS (_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF)
#define CRT_MEMORY_ALWAYS_CHECK_FLAGS (_CRTDBG_CHECK_ALWAYS_DF)
