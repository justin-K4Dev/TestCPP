#include "stdafx.h"


#include <Windows.h>


namespace Introduction
{
	void cpp_what()
	{
        /*

            C++ Introduction

              - C++는 범용(system + application) 프로그래밍 언어로,
                성능과 제어력(메모리/리소스 관리), 그리고 높은 수준의 추상화(클래스, 템플릿)를 동시에 제공한다.

            1) C++는 어떤 언어인가?
              - C 언어를 기반으로 시작해, 객체지향(OOP), 제네릭 프로그래밍(Templates),
                RAII(Resource Acquisition Is Initialization), 함수형 스타일, 모듈/코루틴 등
                여러 패러다임을 흡수하며 발전해왔다.
              - 게임 서버/엔진, 금융, 네트워크, 임베디드, 고성능 컴퓨팅(HPC) 등
                “성능/지연시간/리소스”가 중요한 영역에서 널리 사용된다.

            2) 왜 C++를 배우는가?
              - 빠르다: 컴파일된 네이티브 코드로 실행되며, 비용이 큰 추상화를 피할 수 있다.
              - 저수준 제어: 메모리 배치, 객체 수명, 스레딩/동기화 등 저수준 제어가 가능하다.
              - 강력한 추상화: 템플릿과 표준 라이브러리(STL)를 통해 성능을 해치지 않으면서
                               재사용 가능한 코드를 작성할 수 있다.
              - 생태계: 다양한 플랫폼(Windows/Linux/macOS/콘솔/모바일)에서 돌아가며,
                        오랜 기간 축적된 라이브러리/툴 체인이 풍부하다.

            3) C++ 프로그램이 실행되기까지: 소스 -> 실행 파일
              - C++는 “컴파일 언어”이며, 일반적으로 아래 과정을 거쳐 실행 파일이 만들어진다.

              3.1) 전처리(Preprocessing)
                - #include, #define 같은 지시문을 처리해 실제 컴파일 단위로 확장한다.

              3.2) 컴파일(Compilation)
                - 확장된 소스 코드를 분석해 목적 파일(.obj/.o)을 만든다.
                  이 단계에서 문법 오류/타입 오류가 대부분 잡힌다.

              3.3) 링크(Linking)
                - 여러 목적 파일과 라이브러리를 결합하여 최종 실행 파일(.exe) 또는 라이브러리(.dll/.so)를 만든다.
                  “정의되지 않은 심볼” 같은 오류는 보통 링크 단계에서 발생한다.

            4) 표준(Standard)과 컴파일러
              - C++는 표준(C++11/14/17/20/23/26...)에 따라 기능이 추가/변경된다.
              - 학습/실무에서는 최소 C++17 이상을 권장하고, 최신 기능이 필요하면 C++20/23을 사용한다.
              - 컴파일러(예: MSVC, GCC, Clang)와 표준 라이브러리 구현은 버전에 따라 지원 범위가 다르다.

            5) 첫 번째 C++ 예제
              - 아래 코드는 콘솔에 텍스트를 출력하는 가장 단순한 형태의 프로그램이다.

                #include <iostream>

                int main()
                {
                    std::cout << "Hello, C++!" << std::endl;
                    return 0;
                }

              - main(): 프로그램의 시작점(entry point)
              - std::cout: 표준 출력 스트림
              - return 0: 운영체제에 “정상 종료”를 알리는 관례적인 값

            6) C++에서 특히 중요한 개념들
              - 타입과 값(Value): int, double, std::string, 사용자 정의 타입
              - 스코프와 수명(Lifetime): 객체가 언제 생성되고 언제 파괴되는가
              - RAII(Resource Acquisition Is Initialization): 자원(메모리/파일/락/소켓)을 객체 수명에 묶어 안전하게 관리
              - 표준 라이브러리(STL): vector, string, map, unordered_map, algorithm
              - 예외/오류 처리: 예외, 에러 코드, Result 패턴 등
              - 성능 사고방식: 복사/이동(move), 캐시 친화, 동적 할당 최소화
        */
        {
            std::cout << "Hello, C++!" << std::endl;
            return;

            // main() : 프로그램의 시작점(entry point)
            // cpp_what() : 함수 정의
            // std::cout : 표준 출력 스트림
            // return : 함수의 반환 처리 (값이 없는 경우)
        }
	}

	void Test()
	{
		cpp_what();
	}

}//Introduction