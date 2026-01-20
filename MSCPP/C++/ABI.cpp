#include "stdafx.h"


namespace ABI //(Application Binary Interface)
{
    void CRT_with_ABI()
    {
        /*
            ABI for CRT

            1. ABI(Application Binary Interface)
              + ABI는 소스 레벨(API)이 아니라 바이너리 레벨에서 지켜야 하는 방식 !!!

                대표적으로:
                - 함수 호출 규약(인자 전달, 스택 정리, 레지스터 사용)
                - 이름 맹글링(name mangling) 규칙
                - struct/class 레이아웃(정렬/패딩, vtable 배치)
                - 예외 처리(throw/catch) 방식
                - RTTI(typeid, dynamic_cast) 표현 방식
                - 디버그/릴리즈에 따른 객체/이터레이터 표현
                - 메모리 할당/해제(힙)와 런타임 라이브러리 경계

            2. CRT 관련 ABI(Application Binary Interface)
              2.1. 힙/메모리 관리 ABI
                + CRT가 제공하는 malloc/free, new/delete가 어떤 CRT 힙을 쓰느냐에 따라 달라진다.
                + 서로 다른 CRT(혹은 서로 다른 CRT 모드)로 빌드된 모듈이 섞이면:
                  - DLL A에서 new로 할당한 메모리를
                    DLL B에서 delete 하면
                    서로 다른 힙/런타임 경계를 넘는 해제가 되어 크래시/힙 손상이 발생할 수 있다.
                    그래서 Windows에서 C++ DLL 설계시 “메모리 소유권을 경계 밖으로 넘기지 말라”는 규정이 있다.

              2.2. CRT 링크 방식(/MD vs /MT)
                + /MD : DLL CRT(동적 런타임) 사용 (대개 msvcp*.dll, vcruntime*.dll, ucrtbase.dll 등)
                + /MT : 정적 CRT(런타임을 exe/dll에 포함)
                + 이게 섞이면 모듈마다 CRT가 달라져서 힙/전역 상태(FILE, locale, errno 등) 공유가 꼬일 수 있고,
                  라이브러리 링크 충돌도 나기 쉽다.

              2.3. 디버그 CRT vs 릴리즈 CRT
                + MSVC는 디버그 CRT에서 힙 체크/가드/추적을 넣고, 일부 STL/이터레이터 체크가 켜져서
                  같은 타입이라도 내부 표현(특히 STL)이 달라지거나
                  링크 대상 라이브러리가 서로 다른 런타임을 요구하면 문제가 생긴다.

              2.4. STL/이터레이터/예외 ABI
                +“CRT”라고 부르지만 현실에선 C++ 표준 라이브러리(STL) + 예외 런타임 + vcruntime까지 같이 묶여 ABI 문제를 일으킨다.
                + 예)
                  - DLL 경계를 넘어 std::string, std::vector 같은 STL 타입을 주고받기
                  - 예외를 DLL 경계를 넘어 throw/catch 하기
                + 이건 컴파일러/런타임/빌드 옵션이 다르면 ABI가 깨질 위험이 크다.

            3. 안전한 CRT 사용의 예
              3.1. 같은 프로세스 안에서 함께 링크/로드되는 모든 모듈은
                   가능하면 같은 컴파일러(MSVC 버전), 같은 런타임 옵션(/MD 또는 /MT), 같은 Debug/Release로 맞춘다.

              3.2. DLL 경계를 넘는 API는:
                + C 스타일(plain old data)로 하거나
                + 소유권 규칙을 명확히 하고(누가 free/delete?)
                + 가능하면 할당/해제도 같은 모듈에서 하게 만든다(예: CreateX()/DestroyX() 패턴).

              3.3. STL 타입을 DLL 경계로 직접 노출하지 않는 편이 안전합니다(특히 서로 다른 빌드/툴체인 가능성이 있으면).
        */
    }

	void CRT_LinkType_what()
	{
        /*
            MSVC CRT 링크 옵션 정리: /MT /MTd /MD /MDd


            [1] CRT(C Run-Time)란?

              - malloc/free, new/delete, printf, 파일 I/O, locale, 예외, TLS(Thread Local Storage) 등
                C/C++ 기본 런타임 기능을 제공하는 라이브러리(= CRT).
              - MSVC에서는 CRT를
                * 정적 링크: /MT, /MTd
                * 동적 링크: /MD, /MDd
                중 하나로 선택한다.
              - 옵션 뒤의 'd'는 Debug CRT(디버그 힙/검사/추적 기능 포함)를 의미한다.


            [2] 옵션별 의미

              /MT  (Multi-threaded, Static CRT)
                - 의미: 멀티스레드 CRT를 "정적 링크" (CRT 코드가 EXE/DLL에 포함)
                - 특징:
                  * 배포 단순화 가능(런타임 DLL 의존 감소)
                  * 바이너리 크기 증가
                  * 모듈(EXE/DLL)마다 CRT/힙이 분리될 수 있어 경계에서 위험 증가

              /MTd (Multi-threaded, Static Debug CRT)
                - 의미: /MT + Debug CRT 정적 링크
                - 특징:
                  * 디버그 힙(누수 체크, 가드, CRT 리포트 등) 사용 가능
                  * 디버그 전용(성능 저하/배포 부적합)

              /MD  (Multi-threaded, DLL CRT)
                - 의미: 멀티스레드 CRT를 "동적 링크" (CRT를 DLL로 공유)
                - 특징:
                  * EXE/DLL 크기 감소
                  * 프로세스 내 모듈들이 동일 CRT DLL 공유 → 모듈 간 CRT 일관성 ↑
                  * VC++ Redistributable 필요 가능(배포 정책에 따름)

              /MDd (Multi-threaded, DLL Debug CRT)
                - 의미: /MD + Debug CRT 동적 링크
                - 특징:
                  * 디버그 CRT 기능 사용 가능
                  * 디버그 전용(디버그 런타임 DLL 없으면 실행 불가)


            [3] 실무 핵심 리스크(혼용의 본질)

              (1) 모듈 경계 메모리(힙) 문제
                - 서로 다른 CRT를 쓰는 모듈 간에
                  "할당(A) / 해제(B)"가 교차하면 크래시/힙 손상/누수 위험.
                - /MT는 각 모듈이 CRT를 포함할 수 있어 "각자 다른 힙" 상황이 쉽게 발생.
                - /MD는 CRT DLL을 공유하므로 이런 위험이 상대적으로 줄어듦(그래도 규약 설계가 핵심).

              (2) Debug/Release 혼용 문제
                - /MDd, /MTd(디버그 CRT)와 /MD, /MT(릴리즈 CRT)는 런타임 자체가 다르다.
                - 디버그 라이브러리를 릴리즈 구성에 섞어 링크하면 문제 발생 가능성이 매우 높다.


            [4] 선택 가이드(추천 시나리오)

              (1) 일반 앱/게임/서버(다수 모듈, DLL, 서드파티 라이브러리 사용)
                - 추천: Release=/MD, Debug=/MDd
                - 이유:
                  * 통합 비용이 낮고(서드파티/SDK가 /MD 전제인 경우 다수)
                  * 모듈 간 CRT 일관성 측면에서 유리

              (2) 단일 EXE 지향, 외부 DLL 거의 없음, 배포 단순성이 최우선
                - 고려: Release=/MT
                - 주의:
                  * 나중에 DLL/플러그인 구조가 추가되면 경계 이슈(힙/ABI)가 급증
                  * /MD 기반 서드파티 사용 시 재빌드/호환성 비용 증가 가능

              (3) 라이브러리 제작자(.lib/.dll 배포)
                - 원칙: 소비자 프로젝트 CRT 옵션과 일치시키는 것이 가장 안전
                - 현실적 운영:
                  * 정적 .lib는 /MD용, /MT용을 각각 제공하거나
                  * DLL로 제공 + API 경계를 CRT/ABI 독립적으로 설계하여 리스크를 낮춘다


            [5] 실무 베스트의 예

              (1) 솔루션 전체 CRT 정책 통일
                - 추천: Debug=/MDd, Release=/MD
                - 예외가 필요하면 "API 경계 설계"로 방어

              (2) DLL 경계 규약
                - "할당/해제는 같은 모듈에서" 원칙
                  예) CreateX()/DestroyX(x)
                - 또는 caller-allocated buffer 패턴(호출자가 버퍼 제공, DLL은 채우기만)

              (3) STL을 ABI 경계로 직접 노출 금지(특히 DLL)
                - std::string, std::vector는 CRT 뿐 아니라 컴파일러/옵션(/EH, iterator debug level 등)까지 영향
                - DLL 경계에서는 다음을 권장:
                  * C API(포인터 + 길이)
                  * POD struct
                  * opaque handle + PImpl


            [6] EXE + Library CRT 조합별 “링크 성공 여부”(MSVC 경험칙)

              (A) 정적 라이브러리(.lib): 오브젝트 코드가 EXE/DLL에 직접 포함
                - 원칙: EXE와 static lib는 CRT 옵션을 "맞추는 게 거의 필수"
                - 불일치 시: 대개 LNK2038(RuntimeLibrary mismatch)로 링크 단계에서 차단

                | EXE CRT | Static LIB CRT | 링크 결과(일반)          | 비고
                |---------|----------------|--------------------------|------------------------------
                | /MD     | /MD            | OK                       | 권장(Release)
                | /MDd    | /MDd           | OK                       | 권장(Debug)
                | /MT     | /MT            | OK                       | 단일 EXE 지향시
                | /MTd    | /MTd           | OK                       | Debug 전용
                | /MD     | /MT            | FAIL(대개 LNK2038)       | RuntimeLibrary mismatch
                | /MT     | /MD            | FAIL(대개 LNK2038)       | RuntimeLibrary mismatch
                | Debug/Release 혼용       | FAIL/UNSTABLE(대개 실패) | /MDd<->/MD, /MTd<->/MT 등

              (B) DLL + import lib(.lib): EXE는 DLL 코드가 아니라 import thunk만 링크
                - EXE와 DLL의 CRT가 달라도 "링크 성공"은 가능한 경우가 흔함
                - 하지만 안전성은 런타임 규약(메모리 소유권/예외 전파/STL ABI(Application Binary Interface))에 의해 결정됨

                | EXE CRT | DLL CRT | 링크 결과(일반)  | 핵심 설명
                |---------|---------|------------------|-----------------------------------------
                | /MD     | /MD     | OK               | 가장 일반적
                | /MDd    | /MDd    | OK               | Debug 기준
                | /MT     | /MD     | OK(자주)         | import lib 링크이므로 링크 자체는 가능
                | /MD     | /MT     | OK(자주)         | 동일
                | /MTd    | /MDd    | OK(자주)         | 동일
                | /MDd    | /MTd    | OK(자주)         | 동일

                * 중요:
                  - DLL 경계에서 "메모리 소유권 교차 금지", "예외 전파 금지", "STL 직접 노출 금지"를 지키면
                    혼합 CRT에서도 안전하게 설계할 수 있다.
        */

	}

    void Linkage()
    {
        /*
            extern "C" 이란 ?

              - C++의 "이름 맹글링(name mangling)"을 끄고 C 링크 규약으로 심볼 이름을 고정한다.
              - DLL에서 GetProcAddress("foo")로 찾거나, 다른 언어/컴파일러와 ABI를 맞출 때 유용하다.
              - 주의: extern "C"는 "호출 규약"을 바꾸지 않고, "심볼 이름 규칙"만 바꾼다.

              1. 사용의 예

                lib.h
                [
                    #pragma once

                    int c_add(int a, int b);
                ]

                main.cpp
                [
                    #include <iostream>

                    extern "C" {
                        #include "lib.h"   // C 헤더를 C++에서 include할 때 맹글링 방지
                    }

                    int main() {
                        std::cout << c_add(1, 2) << "\n";
                    }

                    // extern "C"가 없으면 C++ 컴파일러가 c_add()를 C++ 링크 규약으로 보려고 해서 링크를 실패 할 수 있다 !!!
                ]

        */
    }

    void ExportAndImport()
    {
        /*
            __declspec(dllexport)

              - 목적: DLL이 외부로 제공할 함수/변수를 Export Table에 등록 (내보내기).
              - 효과: 다른 모듈(EXE/DLL)에서 import lib 또는 GetProcAddress()로 접근 가능 !!!

            __declspec(dllimport)

              - 목적: 이 심볼은 현재 모듈에 정의된 게 아니라 외부 DLL에서 가져온다고 표시.
              - 효과: 호출이 보통 IAT(Import Address Table) 를 통한 간접 호출 형태로 생성됨.
              - 주의: dllimport로 선언된 심볼을 같은 모듈에서 구현하면 C2491 같은 오류 발생 !!!

               1. 사용의 예

                dll.h
                [
                    #pragma once

                    #ifdef _WIN32
                        #define ABI_CALL __cdecl

                        #ifdef BUILDING_DLL
                            #define DLL_API __declspec(dllexport)
                        #else
                            #define DLL_API __declspec(dllimport)
                        #endif
                    #else
                        #define ABI_CALL
                        #define DLL_API
                    #endif

                    extern "C" {
                        DLL_API int ABI_CALL abi_add(int a, int b);
                        DLL_API int ABI_CALL abi_sub(int a, int b);
                    }
                ]

                dll.cpp
                [
                    #define BUILDING_DLL
                    #include "dll.h"

                    int ABI_CALL abi_add(int a, int b) { return a + b; }
                    int ABI_CALL abi_sub(int a, int b) { return a - b; }
                ]

                main.cpp
                [
                    #include <iostream>
                    #include "dll.h"

                    std::cout << "abi_add(10,20) = " << abi_add(10, 20) << "\n";
                    std::cout << "abi_sub(10, 3) = " << abi_sub(10, 3) << "\n";
                ]
         */
    }

    void CallingConvention()
    {
        /*
            C / C++ Calling Convention(호출 규약) 정리 (Windows/MSVC 중심)

            [1] Calling Convention이란?

              - 함수 호출 시 "바이너리 수준의 약속(ABI)".
              - 아래 항목들을 규정한다:
                * 인자를 어떤 순서/방식으로 전달하는가(스택/레지스터, right-to-left 등)
                * 리턴값을 어디에 두는가(레지스터/메모리)
                * 스택 정리를 누가 하는가(caller cleanup / callee cleanup)
                * (주로 x86) 심볼 이름 장식(name decoration) 규칙

              - 중요: 호출 규약이 서로 다른 함수/함수포인터를 섞으면
                특히 x86에서는 스택이 깨져 크래시/메모리 손상이 날 수 있다.


            [2] Windows/MSVC에서 자주 쓰는 호출 규약

              (1) __cdecl
                - 전통적으로 C 쪽 기본값에 가깝다(특히 MSVC C 컴파일).
                - 스택 정리: 호출자(caller)가 정리.
                - 특징:
                  * 가변 인자(..., printf 스타일)에 적합/관례적으로 사용.
                - 사용 예:
                  int __cdecl add(int a, int b);
                  using Fn = int (__cdecl*)(int,int);

              (2) __stdcall
                - Win32 API에서 전통적으로 많이 사용(특히 32비트).
                - 스택 정리: 피호출자(callee)가 정리.
                - 특징:
                  * API의 호출 규약을 고정하고 싶을 때 사용.
                - 참고:
                  * WINAPI 매크로(32비트)는 보통 __stdcall로 매핑되는 경우가 많다.

              (3) __fastcall
                - (x86) 일부 인자를 레지스터로 전달하여 호출 오버헤드를 줄이려는 목적.
                - 세부 동작은 컴파일러/옵션에 의존할 수 있어
                  외부 공개 ABI 경계에서는 보통 덜 권장.

              (4) __thiscall
                - (x86) C++ 멤버 함수 호출 규약.
                - this 포인터를 보통 ECX 레지스터로 전달.
                - 멤버 함수 포인터/바이너리 경계에서 복잡도가 커질 수 있어
                  공개 ABI로는 C API + 핸들(opaque pointer) 래핑을 권장.

              (5) __vectorcall (MSVC 확장)
                - SIMD/벡터 인자를 레지스터로 더 적극적으로 전달.
                - 수치/벡터 연산 성능 목적.
                - 외부 공개 ABI로는 제한적으로 사용.


            [3] 64비트(x64)에서의 차이점

              - Windows x64 ABI에서는 호출 규약이 사실상 "하나로 통일"되어 있다.
              - 따라서 x64에서는 __cdecl / __stdcall / __fastcall을 붙여도
                실제 호출 규약이 큰 차이가 없는 편이다.
              - 다만 함수 포인터 타입/문서/코드 일관성 목적에서
                ABI_CALL 같은 매크로로 명시해두는 것이 실무적으로 유용하다.

              - 참고: Linux/macOS(System V AMD64 ABI)는 Windows x64와 ABI가 다르므로,
                OS가 다르면 같은 C/C++라도 ABI가 다르다는 점을 유의.


            [4] C vs C++ 관점 핵심

              - C:
                * 오버로드가 없어서 심볼/링킹이 단순.
                * 외부 ABI를 만들 때 호출 규약을 명시하고, (C++에서면 extern "C"도 함께)
                  안정적인 인터페이스를 만들기 좋다.

              - C++:
                * 클래스/템플릿/STL은 ABI가 쉽게 깨질 수 있다.
                * DLL/플러그인 경계에서는 "C ABI 래퍼 + 핸들(opaque pointer)" 패턴 권장.
                * 멤버 함수 규약(__thiscall) 등으로 바이너리 경계가 복잡해질 수 있다.


            [5] 실무 체크리스트(특히 DLL/플러그인/함수포인터/타언어 연동)

              - 공개 API에는 호출 규약을 명시해라.

                  #define ABI_CALL __cdecl
                  
                  extern "C" __declspec(dllexport) int ABI_CALL foo(int);

              - 함수 포인터 타입에도 같은 호출 규약을 넣어라.
                  
                  using FooFn = int (ABI_CALL*)(int);

              - x86에서는 calling convention mismatch가 크래시로 직결될 수 있다.
              - x64에서는 대부분 통일이지만, 문서/일관성/정적 분석을 위해 명시가 유익하다.
        */

    }

	void Test()
	{
        CallingConvention();

        ExportAndImport();

        Linkage();

        CRT_LinkType_what();

		CRT_with_ABI();
	}

}//ABI