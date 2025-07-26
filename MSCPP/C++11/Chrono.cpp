#include "stdafx.h"


#include <vector>
#include <atomic>
#include <ctime>
#include <time.h>
#include <chrono>
#include <Windows.h>


namespace Chrono
{
    void waiting(int waitSec)
    {
        for (int i = 0; i < waitSec; ++i) {
            Sleep(1000);
        }
    }

    void std_chrono_what()
    {
        /*
            📚 std::chrono (C++11)

              - std::chrono는 C++11에서 새롭게 추가된 시간 및 기간(duration) 관련 유틸리티 라이브러리
              - 주요 목적: 고정밀 시간 측정, 코드의 경과 시간 측정, 타임스탬프, 시간 단위 변환, 타이머 구현 등
              - 주요 구성 요소
                * 시계(Clock): system_clock(실시간), steady_clock(일관된 간격), high_resolution_clock(최고 정밀도)
                * 기간(Duration): 특정 단위(초, ms, us 등)의 시간 간격 표현
                * 시간점(Time point): 시계의 특정 시점 표현 (예: now())
                * 시간 단위 변환: duration_cast로 다양한 시간 단위 변환 지원
              - 대표 사용 예
                * 코드 실행시간(성능) 측정 및 벤치마킹
                * 타임스탬프 기록, 주기적 이벤트, 타이머, sleep 등
                * 시간 연산 및 비교

              - <chrono>는 타입 안정성과 단위 일관성을 제공해 기존 C 언어 스타일보다 오류가 줄고,
                현대적 C++ 코드에서 시간/기간 연산을 명확하고 안전하게 작성할 수 있게 해줌
        */
        {
            // 1. 시작 시각 기록
            auto start = std::chrono::system_clock::now();

            // 2. 측정하고자 하는 코드/함수 실행 (여기서는 5초 대기)
            waiting(5);

            // 3. 종료 시각 기록
            auto end = std::chrono::system_clock::now();

            // 4. 경과 시간 계산 (초 단위)
            std::chrono::duration<double> elapsed_seconds = end - start;

            // 5. 종료 시각을 time_t로 변환 (사람이 읽을 수 있는 문자열 출력)
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            char strTime[1024];
            ctime_s(strTime, sizeof(strTime), &end_time);

            std::cout << "finished computation at " << strTime
                      << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;

            /*
            출력:
                finished computation at Sun Jun 24 18:35:48 2018
                elapsed time: 5.00029s
            */

            system("pause");
        }
    }

	void Test()
	{
        std_chrono_what();
	}
}//Chrono