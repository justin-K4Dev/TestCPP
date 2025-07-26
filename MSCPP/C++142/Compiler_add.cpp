#include "stdafx.h"


#include <iostream>
#include <tuple>


namespace Compiler_AddFeatures
{
    // C++14: auto 리턴 타입, 구조적 바인딩(구조 분해) C++17~
    auto get_pair() {
        return std::make_tuple(1, 2.5);
    }

    void Compiler_AddSwitches()
    {
        /*
            📚 MSVC(Visual Studio) 주요 최신 컴파일러 스위치 개요

            - /std:c++17, /std:c++20, /std:c++latest
                : 원하는 C++ 표준의 언어 기능/라이브러리를 명확히 활성화
                : 최신 draft 기능(C++2a/2b/2c 등)은 /std:c++latest에서만 동작
                : 기본값은 프로젝트 종류/버전에 따라 다름(명시 지정 권장)

            - /permissive-
                : 표준 C++과의 엄격한 호환성 유지(비표준 문법/확장 대부분 비활성화)
                : 레거시 MSVC 확장으로 인한 버그 방지, 크로스 플랫폼 호환성↑

            - /diagnostics
                : 진단 메시지(에러/경고) 출력 시, 라인/컬럼 정보 및 강조 커서 표시
                : 대형 프로젝트 디버깅, 오프라인 에러 분석 등에 매우 편리

            - /debug:fastlink
                : 링크 타임에 전체 디버그 정보 복사 생략, PDB파일 참조 방식으로 빌드 속도 대폭 개선
                : 대규모 프로젝트에서 반복 빌드/디버깅 효율화

            - /sdl, /await, /RTC
                : /sdl(안전한 개발 라이프사이클), /await(C++ Coroutines), /RTC(RunTime Checks) 등의 조합에 대한
                  제한 완화(예: 코루틴 기능 활성화 시 /RTC 적용 가능)

            참고:
              https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version
              https://learn.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance
              https://learn.microsoft.com/en-us/cpp/build/reference/diagnostics
              https://learn.microsoft.com/en-us/cpp/build/reference/debug-debugging-options
        */

        {
            // 구조적 바인딩 (C++17~)
            auto [a, b] = get_pair();
            std::cout << "a: " << a << ", b: " << b << std::endl; // a: 1, b: 2.5

            // C++17: if constexpr (컴파일 타임 분기)
            if constexpr (sizeof(void*) == 8)
                std::cout << "64비트" << std::endl;
            else
                std::cout << "32비트" << std::endl;

            /*
            출력:
                a: 1, b: 2.5
                64비트   (또는 32비트)
            */
        }
    }

    void Test()
    {
        Compiler_AddSwitches();
    }

}//Compiler_AddFeatures