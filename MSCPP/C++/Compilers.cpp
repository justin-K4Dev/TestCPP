#include "stdafx.h"


#include <Windows.h>


namespace Compilers
{
    /*
        C++ 컴파일러 개요

        1. 컴파일러(Compiler)란?
          - 사람이 읽는 C++ 소스 코드를 CPU가 실행할 수 있는 기계어(오브젝트 코드)로 변환하는 프로그램.
          - 보통 다음 단계를 거친다.

        2. 빌드 파이프라인(큰 흐름)
          (1) 전처리(Preprocess)
              - #include, #define, #if 등을 처리하여 "하나의 번역 단위(translation unit)"를 만든다.
          (2) 컴파일(Compile)
              - C++ 문법/의미 분석 후 .obj(오브젝트 파일) 생성.
          (3) 링크(Link)
              - 여러 .obj와 라이브러리(.lib)를 합쳐 최종 실행 파일(.exe) 또는 DLL(.dll)을 만든다.
              - 이 과정에서 "정의되지 않은 심볼(unresolved external symbol)"이 해결된다.

        3. 오브젝트/라이브러리의 역할
          - .obj : 각 cpp가 컴파일된 결과물(기계어 조각 + 심볼 정보)
          - .lib : 정적 라이브러리(여러 obj를 묶은 것). 링크 시 exe/dll에 코드가 포함될 수 있다.
          - .dll : 동적 라이브러리(실행 시 로드). exe는 보통 import lib(.lib)을 통해 호출 진입점을 연결한다.
    */

    void overview()
    {
        /*
            아래 출력은 "빌드 도구"가 실제로 하는 일을 사람이 이해하기 쉽게 표현한 것이다.
            (실제 동작은 컴파일러/링커가 수행하며, 이 함수는 그 개념을 요약해준다.)
        */
        std::cout << "[C++ Build Overview]\n";
        std::cout << "1) Preprocess : #include/#define resolved -> translation unit\n";
        std::cout << "2) Compile    : C++ -> .obj (machine code + symbols)\n";
        std::cout << "3) Link       : .obj + .lib -> .exe/.dll (resolve externals)\n";
        std::cout << std::endl;
    }

    //---------------------------------------------------------------------------------------------
    // 링크 단계 개념: "선언(Declaration) vs 정의(Definition)"
    //  - 선언만 있고 정의가 없으면 링크 에러가 난다(LNK2019 등)
    //  - 아래 코드는 "정상 예제"로, 정의까지 포함한다.
    //---------------------------------------------------------------------------------------------
    namespace LinkageExample
    {
        // 선언(Declaration)
        int add(int a, int b);

        // 정의(Definition)
        int add(int a, int b)
        {
            return a + b;
        }

        void run()
        {
            int r = add(3, 4);
            std::cout << "[LinkageExample] add(3,4) = " << r << "\n";
        }
    }

    //---------------------------------------------------------------------------------------------
    // 오버로드와 이름 맹글링(name mangling) 개념
    //  - C++에서는 함수 오버로드가 가능하므로 링커 심볼이 타입 정보를 포함하도록 변형될 수 있다.
    //  - extern "C"를 쓰면 이름 맹글링을 끄고 C 링크 규약으로 고정한다
    //---------------------------------------------------------------------------------------------
    namespace ManglingExample
    {
        int add(int a, int b) { return a + b; }
        double add(double a, double b) { return a + b; }

        // C 링크 규약(오버로드 불가: 같은 이름으로는 1개만 가능)
        extern "C" int c_add(int a, int b) { return a + b; }

        void run()
        {
            std::cout << "[ManglingExample] add(int) = " << add(1, 2) << "\n";
            std::cout << "[ManglingExample] add(double) = " << add(1.5, 2.5) << "\n";
            std::cout << "[ManglingExample] c_add = " << c_add(10, 20) << "\n";
        }

        /*
            1. 맹글링(name mangling)이란?
              
              - C++ 컴파일러가 링커가 구분할 수 있도록 함수/변수의 “링커 심볼 이름”을 인코딩해서 바꾸는 것이야.
              
              * 왜 바꾸냐면 C++에는 다음이 있기 때문:

                - 오버로드(같은 함수명, 다른 파라미터)
                - 네임스페이스/클래스 멤버
                - 템플릿/특수화 등

                그래서 소스에서 이게:

                    namespace N {
                        int add(int, int);
                    }

                C++ 링커 심볼 이름은 컴파일러가 대략 아래와 같은 형태로 인코딩 한다.

                    MSVC 예: ?add@N@@YAHHH@Z
                    GCC/Clang(Itanium) 예: _ZN1N3addEii

                즉, “N::add(int,int)”라는 정보를 문자열로 인코딩한 결과물이야.

            2. "맹글링 방지"가 정확히 뭘 막는 거냐?

              - extern "C"를 쓰면 C++ 맹글링 규칙을 적용하지 말고, C 방식 링커 심볼 규칙(C linkage)을 쓰라는 의미 !!!

              * 즉, 이 선언:

                  extern "C" int add(int, int);

                은 컴파일러에게:

                  "이 함수의 링커 심볼 이름을 C처럼 만들고"
                  "C++식으로 ?add@@... 같은 이름으로 변형하지마"

                라고 지시하는 것 !!!

              - 결과적으로 export/링커 심볼이 보통 그냥:
                add (혹은 플랫폼에 따라 _add 같은 단순 접두어 정도)
                처럼 안정적이고 예측 가능한 이름이 된다.

            3. 왜 이게 중요해?
              (1) DLL export 이름을 사람이/다른 언어가 찾을 수 있게

                GetProcAddress(h, "add")는 문자열 "add"로 심볼을 찾는데,
                C++ 맹글링이 걸리면 DLL에는 "add"가 아니라 "?add@@YAHHH@Z" 같은 이름이 들어가서 못 찾음.

                ❌ (extern "C" 없음) GetProcAddress("add") 실패 가능
                ✅ (extern "C" 있음) GetProcAddress("add") 성공

              (2) 컴파일러/옵션이 바뀌면 심볼 이름도 바뀜(=취약)

                C++ 맹글링 포맷은 컴파일러마다 다르고, 같은 컴파일러라도 버전/옵션에 따라 달라질 수 있어.
                그래서 C++ 이름을 그대로 ABI 경계로 쓰면 “바이너리 호환”이 깨지기 쉬움.

                - extern "C"로 이름을 단순화하면 이 위험이 크게 줄어.

            4. 주의: 맹글링 방지는 “이름”만 고정한다

              * 중요한 포인트:

                - extern "C"는 호출 규약(스택 정리, 레지스터 사용) 을 바꾸는 게 아니라
                  링커 심볼 이름 규칙만 바꾼다.
                  그래서 DLL ABI를 정말 고정하려면 보통 같이 명시해:

                    extern "C" __declspec(dllexport) int __cdecl add(int, int);

            5. 한 줄로 정리
              - "맹글링 방지" 란 extern "C"로 C++ 컴파일러의 이름 변형(name mangling)을 끄고, 
                DLL/링커 심볼 이름을 ‘add’처럼 단순하고 예측 가능하게 고정하는 것을 의미 !!!
        */
    }

    //---------------------------------------------------------------------------------------------
    // 라이브러리(.lib) 개념을 위한 형태
    //  - 이 파일이 static lib로 빌드되고, exe가 링크한다고 가정하면
    //    아래의 "lib_function"들이 라이브러리 API가 된다.
    //  - 실제 static lib 분리는 프로젝트 구성에서 수행(코드 자체는 동일).
    //---------------------------------------------------------------------------------------------
    namespace StaticLibShape
    {
        int lib_sum_range(int start, int count)
        {
            int s = 0;
            for (int i = 0; i < count; ++i)
                s += (start + i);
            return s;
        }

        void run()
        {
            std::cout << "[StaticLibShape] sum_range(1,5) = "
                << lib_sum_range(1, 5) << "\n"; // 1+2+3+4+5=15
        }
    }

    //---------------------------------------------------------------------------------------------
    // 컴파일러/링커 옵션이 만든 결과를 확인하는 간단 출력
    //  - _MSC_VER : MSVC 버전
    //  - _DEBUG   : Debug 빌드 여부
    //  - _DLL     : /MD(/MDd) 여부(동적 CRT)
    //  - _MT      : 멀티스레드 CRT 여부(대부분 켜짐)
    //---------------------------------------------------------------------------------------------
    void print_build_macros()
    {
        std::cout << "[Build Macros]\n";

#ifdef _MSC_VER
        std::cout << "  _MSC_VER = " << _MSC_VER << "\n";
#else
        std::cout << "  _MSC_VER = (not MSVC)\n";
#endif

#ifdef _DEBUG
        std::cout << "  _DEBUG   = defined (Debug)\n";
#else
        std::cout << "  _DEBUG   = not defined (Release)\n";
#endif

#ifdef _DLL
        std::cout << "  _DLL     = defined (/MD or /MDd)\n";
#else
        std::cout << "  _DLL     = not defined (/MT or /MTd)\n";
#endif

#ifdef _MT
        std::cout << "  _MT      = defined (multi-threaded CRT)\n";
#else
        std::cout << "  _MT      = not defined\n";
#endif

        std::cout << std::endl;
    }

    //=============================================================================================

    void Test()
    {
        // 빌드 매크로 확인
        print_build_macros();

        // static lib 형태 예시
        StaticLibShape::run();

        // 오버로드/extern "C" 개념
        ManglingExample::run();

        // 선언/정의/링크 개념
        LinkageExample::run();

        // 개요 출력
        overview();
    }

}//Compilers