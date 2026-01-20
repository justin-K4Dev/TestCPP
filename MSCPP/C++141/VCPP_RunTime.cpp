#include "stdafx.h"


#include <windows.h>
#include <cfguard.h> // CFG 관련 심볼 포함 (Windows 10 SDK 이상)


namespace VCPP_RunTime
{
	void Gauard_use()
	{
        /*
            📚 Visual C++ Runtime: cfguard.h와 Control Flow Guard(CFG) 개요

              - Control Flow Guard(CFG)는 마이크로소프트가 제공하는 런타임 보안 기술
                → 런타임에서 함수 포인터, 간접 분기 등 "잘못된 코드 경로 실행"을 차단하여
                   버퍼 오버플로우, ROP 공격 등 메모리 공격을 방지
              - cfguard.h: CFG 관련 심볼, 구조체, 인터페이스 선언 제공
                → __guard_check_icall, __guard_dispatch_icall, IMAGE_GUARD_CF_* 등 다양한 CFG 메타데이터 및 런타임 심볼 포함
                → /guard:cf (Visual Studio 빌드 옵션)과 연동되어 작동
              - CFG는 Visual Studio 2015 Update 3 이후 기본 제공,
                최신 SDK에서는 cfguard.h 헤더로 관련 API 심볼 및 진단 가능
              - 자세한 정보: https://learn.microsoft.com/en-us/cpp/build/reference/guard-enable-control-flow-guard

              실행
                - cl /guard:cf myfile.cpp

            📌 코드 설명
              - cfguard.h를 인클루드 하면 IMAGE_GUARD_CF_*, _GUARD_CHECK_ICALL 등의 심볼을 코드에서 확인 가능
              - /guard:cf 옵션이 실제로 켜져 있으면,
              - Release/배포 빌드에서 Visual Studio의 PE 헤더(프로그램 속성→보안)에서 "Control Flow Guard"가 적용된 것 확인 가능
              - 실전에서는 DLL injection, 함수 포인터 변조 등 공격을 차단함
        */

#ifdef IMAGE_GUARD_CF_INSTRUMENTED
        std::cout << "cfguard.h is available. Control Flow Guard symbols are defined." << std::endl;
#endif

#if defined(_GUARD_CHECK_ICALL)
        std::cout << "_GUARD_CHECK_ICALL macro is defined." << std::endl;
#endif

        // PE 헤더에서 CFG가 활성화되었는지 런타임 진단 (실무에선 실제 PE 검사 툴 필요)
        // 간단히는 아래처럼 사용할 수 있음:
        std::cout << "CFG 테스트: 함수 포인터 사용 중" << std::endl;
        typedef void(*FuncPtr)();
        FuncPtr fp = [] { std::cout << "함수 포인터 호출" << std::endl; };
        fp();

        /*
            출력:
            cfguard.h is available. Control Flow Guard symbols are defined.
            CFG 테스트: 함수 포인터 사용 중
            함수 포인터 호출
        */
	}

	void Test()
	{
        Gauard_use();
	}

}//VCPP_RunTime