#include "stdafx.h"

import CustomModule;

#include <iostream>


namespace Modules
{
    void module_use()
    {
        /*
            Module

            - C++20에서 도입된 새로운 코드 분할 및 컴파일 단위 기능
            - 전통적인 #include 헤더 방식의 단점을 개선 (중복 포함, 컴파일 속도 저하, 매크로 오염 등)
            - 컴파일러가 모듈별로 명확하게 인터페이스(공개)와 구현(비공개) 구분
            - 모듈은 export(공개), module(정의), import(사용) 키워드로 관리
            - 빌드 속도 개선, 코드 의존성 및 캡슐화 강화, ODR(One Definition Rule) 위반 방지
            - 확장자: 보통 .ixx, .cppm, .mxx 등 (컴파일러마다 다름)
            - 빌드 시 반드시 모듈 파일을 먼저 컴파일해야 import가 가능
            - 지원 컴파일러: MSVC, GCC(13+), Clang(16+) 등
            - <module> 헤더 필요 없음 (언어 기본 기능)

            [cl 명령어로 모듈 컴파일시 주의사항]
              - Visual Studio의 "Developer Command Prompt for Visual Studio" 또는
                "x64 Native Tools Command Prompt for VS" 콘솔에서 cl 명령어 사용해야 함
              - 일반 cmd.exe 또는 PowerShell에서 실행할 경우 cl 명령 인식 불가
              - 시작 메뉴에서 "Developer Command Prompt" 검색하여 실행
              - 해당 콘솔 진입 후 아래와 같이 컴파일:
                  cl /std:c++20 /c custom_module.ixx
                  cl /std:c++20 main.cpp custom_module.obj`


            [C++20 Module 빌드 과정 개요]

            1. 모듈 소스 작성
                - 예시 파일: CustomModule.ixx
                - 내용:
                    export module CustomModule;
                    export int add(int a, int b) { return a + b; }
                    export int sub(int a, int b) { return a - b; }

            2. 모듈 사용 소스 작성
                - 예시 파일: Module.cpp
                - import CustomModule;
                  ... add, sub 함수 사용 ...

            3. 빌드 과정 (Visual Studio IDE 기준)
                - CustomModule.ixx 파일을 반드시 "소스 파일"로 프로젝트에 추가
                - 프로젝트 속성에서 C++ 언어 표준을 C++20(/std:c++20) 이상으로 설정
                - 빌드 시 VS가 모듈을 먼저 컴파일(.ifc, .obj 생성) 후,
                  나머지 소스와 함께 자동으로 링크하여 실행파일 생성

            4. 빌드 과정 (명령줄/Developer Command Prompt 기준)
                # 1단계: 모듈 소스(.ixx)를 먼저 오브젝트(.obj)로 컴파일
                    cl /std:c++20 /c CustomModule.ixx
                    // CustomModule.obj, CustomModule.ifc 생성됨

                # 2단계: main.cpp/Module.cpp 등 사용 파일을
                         위에서 생성한 obj와 함께 컴파일 및 링크
                    cl /std:c++20 main.cpp Module.cpp CustomModule.obj
                    // main.exe 생성

                - 이때 .ixx 파일을 직접 main.cpp와 같이 컴파일하지 않음!
                - 반드시 obj로 먼저 만들고 obj 파일만 포함

            5. 주요 주의사항
                - "헤더 파일"로 추가하면 빌드/링크 대상이 되지 않으니 반드시 "소스 파일"로 추가
                - export된 함수만 import한 곳에서 사용 가능
                - 빌드 시 모듈과 사용하는 cpp 파일을 obj 단계에서 모두 포함해야 unresolved external symbol 에러 방지

            6. 빌드 에러 예시 및 해결법
                - 모듈 obj가 링크되지 않으면: LNK2019(확인할 수 없는 외부 기호)
                - .ixx 파일을 헤더 파일로 추가했거나, obj 빌드 누락 시 발생

            7. 결과
                - 빌드 성공 시, import한 모듈의 함수(add, sub 등)를 main.cpp 등에서 정상 사용 가능

            ----------------------------------------------------------------

            [실제 빌드 명령 예시(명령줄)]
                cl /std:c++20 /c CustomModule.ixx
                cl /std:c++20 main.cpp Module.cpp CustomModule.obj

            [Visual Studio IDE]
                - 소스 파일로 .ixx 추가
                - 빌드 → 자동 관리

            ----------------------------------------------------------------

            [실행 결과 예시]
                add(3, 2) = 5
                sub(7, 4) = 3
        */

        std::cout << add(3, 2) << std::endl;  // 출력: 5
        std::cout << sub(7, 4) << std::endl;  // 출력: 3
    }

    void Test()
    {
        //module_use();
    }
}