#include "stdafx.h"


#include <string_view>


namespace VCPP_Improvements
{
    // 1. 코드 생성 최적화: 상수 나눗셈 반복문, memset 패턴
    void codegen_optimization() {
        std::vector<int> arr(16);
        // memset 패턴 (최적화 검사)
        std::memset(arr.data(), 0, arr.size() * sizeof(int));
        int sum = 0;
        for (int i = 0; i < 100; ++i) {
            sum += i / 7; // 상수 나눗셈, 자동 벡터화 대상
        }
        std::cout << "[코드 생성] 반복문 상수 나눗셈 합계: " << sum << std::endl;
    }

    // 2. 보안: 버퍼 오버런 진단(컴파일러 경고, 실행시 위험. 실전 사용 금지)
    void buffer_overrun() {
        char buf[8];
        // Visual C++에서 /GS 옵션, 정적 분석에서 경고 발생
        //std::strcpy(buf, "123456789"); // 오버런 버그 (실습 외에는 주의)
        std::cout << "[보안] 버퍼 내용: " << buf << std::endl;
    }

    // 3. 버전 관리: _MSC_VER 매크로 값 확인
    void show_msc_ver() {
        std::cout << "[버전] _MSC_VER: " << _MSC_VER << std::endl;
    }

    // 4. 진단: 라인/컬럼 정보가 포함된 에러 발생 테스트
    void diagnostics() {
        // 아래 코드의 주석을 해제하면, 컴파일 에러 시 라인/컬럼 정보가 진단 메시지에 표시됨
        // int a = "이건 int가 아님"; // 일부러 타입 오류
    }

	void VCPP_Improvements()
	{
        /*
            📚 Visual C++ 최신 릴리즈 주요 개선사항

              - 코드 생성(Codegen) 최적화:
                • 루프 내 연산(특히 상수 정수 나눗셈 등)의 자동 벡터화(vectorization) 향상
                • memset 패턴 탐지 및 변환이 더 똑똑해짐(성능/최적화↑)

              - 코드 보안(Security) 강화:
                • 버퍼 오버런(buffer overrun) 진단 강화
                • /guard:cf (Control Flow Guard) 스위치 사용시, switch문 jump table 코드도 보호

              - 버전 관리(Versioning):
                • 내장 전처리기 매크로 _MSC_VER의 값이 툴셋 업데이트마다 증가(정확한 버전 판별 가능)
                • 여러 버전의 컴파일러/빌드 도구를 PC에 나란히 설치 가능(new toolset layout)

              - 진단 메시지(Diagnostics) 개선:
                • 에러 위치가 파일:라인 뿐만 아니라 "컬럼"까지 상세 표시
                • 빌드/오류 분석, IDE 연동성 개선

              - 참고: Visual C++ Compiler Version, C++ compiler diagnostics improvements, Control Flow Guard, Visual C++ Team Blog 등
        */

        codegen_optimization();

        // 버퍼 오버런: 실제 실행하면 위험하므로 실습 외에는 주의!
        // buffer_overrun();

        show_msc_ver();

        // 진단 예제: 에러 메시지에서 라인/컬럼 확인 (주석 해제)
        // diagnostics();
	}

	void Test()
	{
        VCPP_Improvements();
	}

}//VCPP_Improvements