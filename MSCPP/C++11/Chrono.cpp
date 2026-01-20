#include "stdafx.h"


#include <Windows.h>


#include <thread>


namespace Chrono
{
    template <typename Clock>
    void print_clock_info(const char* name)
    {
        std::cout << "=== " << name << " ===\n";
        std::cout << "is_steady: " << (Clock::is_steady ? "true" : "false") << "\n";
        std::cout << "period: " << Clock::period::num << "/" << Clock::period::den
            << " seconds per tick\n";

        auto t1 = Clock::now();
        auto t2 = Clock::now();
        std::cout << "now() delta (ticks): "
            << std::chrono::duration_cast<typename Clock::duration>(t2 - t1).count()
            << "\n\n";
    }

    template <typename Clock>
    void measure_sleep(const char* name)
    {
        using namespace std::chrono;

        std::cout << "--- measure_sleep with " << name << " ---\n";
        auto start = Clock::now();

        std::this_thread::sleep_for(150ms);

        auto end = Clock::now();
        auto elapsed_us = duration_cast<microseconds>(end - start).count();

        std::cout << "slept ~150ms, measured: " << elapsed_us << " us\n\n";
    }

    void waiting(int waitSec)
    {
        for (int i = 0; i < waitSec; ++i) {
            Sleep(1000);
        }
    }

    std::string format_tm(const std::tm& t, const char* fmt)
    {
        char buf[128];
        if (std::strftime(buf, sizeof(buf), fmt, &t) == 0)
            return "<strftime failed>";

        return std::string(buf);
    }

    //=============================================================================================

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
            using steady = std::chrono::steady_clock;
            using high = std::chrono::high_resolution_clock;
            using sys = std::chrono::system_clock;

            print_clock_info<steady>("steady_clock");
            print_clock_info<high>("high_resolution_clock");
            print_clock_info<sys>("system_clock");

            measure_sleep<steady>("steady_clock");
            measure_sleep<high>("high_resolution_clock");
            measure_sleep<sys>("system_clock");
        }

        system("pause");
    }

    //=============================================================================================

    void system_clock_use()
    {
        /*
            📚 std::chrono::system_clock (C++11)

              - “현재 날짜/시간”을 표현하는 시계
              - 사람이 보는 시간(로그 타임스탬프, 날짜 계산, UTC/local 변환)에 적합
              - monotonic 보장 없음(대부분 is_steady == false)
                → NTP 보정/사용자 변경 등으로 시간이 튈 수 있어서 타임아웃/딜레이 측정용으로 부적합
        */

        using namespace std::chrono;

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
        }

        {
            // 1) system_clock의 now() 얻기
            system_clock::time_point now = system_clock::now();

            // 2) time_t로 변환 (초 단위)
            std::time_t tt = system_clock::to_time_t(now);

            // 3) 로컬 시간 출력
            {
                std::time_t now = std::time(nullptr);

                std::tm tm_local{};
                if (!localtime_s(&tm_local, &now)) return;

                char buf[64];
                std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_local);
                std::cout << "[Local] " << buf << "\n";
            }

            // 4) UTC 출력
            {
                std::time_t now = std::time(nullptr);
                std::tm tm_utc{};
                if (!gmtime_s(&tm_utc, &now)) return;

                char buf[64];
                std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_utc);
                std::cout << "[ UTC ] " << buf << "\n";
            }
        }

        system("pause");
    }

    //=============================================================================================

    bool wait_until_ready_with_timeout(std::chrono::milliseconds timeout)
    {
        typedef std::chrono::steady_clock clock;

        clock::time_point deadline = clock::now() + timeout;

        while (clock::now() < deadline)
        {
            // 어떤 조건을 폴링한다고 한다면 ~
            bool ready = false;
            if (ready)
                return true;

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        return false; // timeout
    }

    void steady_clock_use()
    {
        /*
            📚 std::chrono::steady_clock (C++11)

              - 단조 증가(monotonic) 시계: 시간이 “뒤로 가지” 않는다 !!!
                → 시스템 시간이 바뀌어도(수동 변경, NTP 보정, 서머타임 등) 측정값이 튀거나 역행하지 않게 설계됨.
              - 그래서 경과시간(Elapsed time) 측정, 타임아웃/재시도/쿨타임, 프로파일링에 가장 적합 !!!
              - steady_clock::is_steady == true 인 것이 표준적으로 기대되는 포인트.

              * 요약: “기간 측정/타임아웃은 steady_clock이 정석
        */

        using clock = std::chrono::steady_clock;

        // stopwatch
        {
            clock::time_point t0 = clock::now();

            // 작업 시뮬레이션
            std::this_thread::sleep_for(std::chrono::milliseconds(123));

            clock::time_point t1 = clock::now();

            long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            std::cout << "[stopwatch] elapsed = " << ms << " ms\n";
        }

        // Timeout loop: deadline 기반 타임아웃 폴링
        {
            bool ok = wait_until_ready_with_timeout(std::chrono::milliseconds(200));
            std::cout << "[timeout] " << (ok ? "ready\n" : "timeout\n");
        }

        // Fixed tick: sleep_for 누적오차(드리프트) 줄이기
        {
            const std::chrono::milliseconds tick(50);

            clock::time_point next = clock::now() + tick;

            for (int i = 0; i < 10; ++i)
            {
                // (1) 작업
                std::cout << "[tick] " << i << "\n";

                // (2) 다음 목표 시각까지 대기
                std::this_thread::sleep_until(next);

                // (3) 다음 목표 시각 갱신 (드리프트 방지)
                next += tick;
            }
        }

        // 남은 시간 계산
        {
            typedef std::chrono::steady_clock clock;

            clock::time_point deadline = clock::now() + std::chrono::milliseconds(300);

            // 100ms 작업했다고 가정
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            clock::time_point now = clock::now();
            if (now < deadline)
            {
                long long remain_ms =
                    std::chrono::duration_cast<std::chrono::milliseconds>(deadline - now).count();
                std::cout << "[remain] " << remain_ms << " ms left\n";
            }
            else
            {
                std::cout << "[remain] already timed out\n";
            }
        }

        system("pause");
    }

    //=============================================================================================

    void high_resolution_clock_use()
    {
        /*
            📚 std::chrono::high_resolution_clock (C++11)

              - 가장 높은 분해능(해상도)의 clock !!!
              - 구현에 따라 high_resolution_clock이 steady_clock의 alias 또는 system_clock의 alias일 수 있다 !!!
              - steady_clock::is_steady == true 인 것이 표준 !!!
              - 짧은 구간 벤치마크(마이크로초/나노초 단위 측정)에 사용
              - 그래서 경과시간(Elapsed time) 측정, 타임아웃/재시도/쿨타임, 프로파일링에 가장 적합 !!!

              [필독]
                - steady_clock::is_steady 는‘어떤 환경/라이브러리로 빌드했는가’에 따라 값이 달라질 수 있다 !!!
                  그래서 “항상 true일 거다”라고 가정하면 이식성 문제가 생길 수 있음
                - 플랫폼/표준라이브러리 구현에 따라 값이 다를 수 있다
                - Windows + MSVC STL, Linux + libstdc++, macOS + libc++ 같은 조합이 다르고,
                  같은 OS라도 표준라이브러리/버전/설정에 따라
                  high_resolution_clock이 어떤 clock을 alias하느냐가 달라질 수 있습니다.
                  그래서 어떤 환경에선 true, 다른 환경에선 false가 됩니다.
        */

        using clock = std::chrono::high_resolution_clock;

        // high_resolution_clock 정보 출력
        {
            std::cout << "=== high_resolution_clock info ===\n";
            std::cout << "is_steady: " << (clock::is_steady ? "true" : "false") << "\n";
            std::cout << "period   : " << clock::period::num << "/" << clock::period::den << " sec per tick\n";

            // 대략적으로 "1 tick이 몇 ns인가" 같은 감을 보고 싶으면
            // (정확한 변환은 ratio 기반이라 사람이 읽기만 용도)
            double tick_ns = (1e9 * (double)clock::period::num) / (double)clock::period::den;
            std::cout << "approx tick: " << tick_ns << " ns\n\n";
        }

        // 경과 시간 측정 출력
        {
            clock::time_point t0 = clock::now();

            // 측정 대상 코드(예: 10ms sleep)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            clock::time_point t1 = clock::now();

            long long us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

            std::cout << "[high_resolution_clock] elapsed = " << us << " us\n";

            // is_steady가 false면 경과시간 측정이 "항상 안전"하진 않다는 경고
            if (!clock::is_steady)
            {
                std::cout << "WARNING: high_resolution_clock is not steady on this platform.\n"
                    "         For timeouts/elapsed-time logic, prefer steady_clock.\n";
            }
        }

        system("pause");
    }

    //=============================================================================================

    //---------------------------------------------------------------------------------------------
    // 공통: system_clock::time_point -> 문자열(로컬 시각)
    //---------------------------------------------------------------------------------------------
    static std::string format_system_time_local(std::chrono::system_clock::time_point tp)
    {
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);

        std::tm tmLocal{};

#if defined(_MSC_VER)
        localtime_s(&tmLocal, &tt);
#else
        // POSIX: localtime_r
        localtime_r(&tt, &tmLocal);
#endif

        std::ostringstream oss;
        oss << std::put_time(&tmLocal, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    void time_point()
    {
        /*
            📚 std::chrono 핵심 3요소: time_point (C++11)

              - time_point는 “시각(타임스탬프)”이다.
              - 특정 clock에 소속된 값이며,
                epoch로부터의 duration(오프셋)을 들고 있다.

                std::chrono::time_point<Clock, Duration>

              - 핵심 API:
                tp.time_since_epoch()   -> duration 반환 (epoch부터 얼마나 지났는지)
                Clock::now()            -> 현재 시각(time_point)을 얻음

              - 연산 규칙(매우 중요):
                ✅ time_point + duration = time_point      (미래/과거 시각 이동)
                ✅ time_point - duration = time_point
                ✅ time_point - time_point = duration      (두 시각 차이 = 경과시간)
                ❌ (원칙적으로) 서로 다른 Clock의 time_point끼리 직접 연산 불가
        */

        // 경과 시간 계산
        {
            using namespace std::chrono;

            std::cout << "=== Time point ===\n";

            // (A) system_clock: 현재 "현실 시각"
            auto now_sys = system_clock::now();
            std::cout << "system_clock now(local) = " << format_system_time_local(now_sys) << "\n";

            // 미래 시각 계산: now + 5 seconds
            auto after_5s = now_sys + seconds(5);
            std::cout << "after 5 seconds(local)  = " << format_system_time_local(after_5s) << "\n";

            // (B) steady_clock: 경과시간 측정(프로파일링/타임아웃용)
            auto t0 = steady_clock::now();
            // ... 어떤 작업 ...
            auto t1 = steady_clock::now();

            auto elapsed_us = duration_cast<microseconds>(t1 - t0).count();
            std::cout << "steady elapsed (us)     = " << elapsed_us << "\n\n";
        }

        // 경과 시간 계산 및 체크
        {
            using namespace std::chrono;

            std::cout << "\n==================== [deadline(시각) + timeout(duration)] ====================\n";

            // "현재 시각" + "시간 길이" = "미래 시각(데드라인)"
            steady_clock::time_point start = steady_clock::now();
            steady_clock::time_point deadline = start + milliseconds(300);

            // 바쁜 작업을 흉내내며 deadline 체크
            int loops = 0;
            while (steady_clock::now() < deadline) {
                ++loops;
                // 실제로는 작업 수행...
                // 여기선 너무 빡세지 않게 약간 쉼
                std::this_thread::sleep_for(milliseconds(50));
            }

            auto elapsed_ms = duration_cast<milliseconds>(steady_clock::now() - start).count();
            std::cout << "loops = " << loops << "\n";
            std::cout << "elapsed ~= " << elapsed_ms << " ms\n";
            std::cout << "설명: (steady_clock) deadline = now + timeout 으로 타임아웃 로직 구성\n";
        }

        system("pause");
    }

    //=============================================================================================

    void epoch()
    {
        /*
            📚 std::chrono 핵심 3요소: epoch (C++11)

              - "그 clock이 기준으로 삼는 0 시각(원점)"
              - epoch 자체를 직접 만지는 일은 거의 없고,
                보통 time_point가 epoch 기준 오프셋(time_since_epoch)을 들고 있다.

              - 중요한 점:
                ✅ epoch는 clock마다 다르다.
                ✅ 서로 다른 clock의 time_point는 의미가 다르고 직접 비교/연산이 불가하다(원칙적으로).

              * 대표적인 예:
                std::chrono::system_clock
                  - epoch가 흔히 "Unix epoch(1970-01-01 00:00:00 UTC)"와 대응되도록 구현되는 경우가 많지만,
                    표준이 “반드시 1970”이라고 강제하진 않는다.
                  - 다만 대부분 구현이 time_t 변환(to_time_t)을 제공하므로 현실 시각과 연결된다.

                std::chrono::steady_clock
                  - epoch는 "프로세스 시작 시점" 같은 내부 기준일 수도 있고,
                    OS의 monotonic time 기준일 수도 있다.
                  - 절대시각 의미가 없고, 오직 "경과시간"용이다.

                std::chrono::high_resolution_clock
                  - 별도 clock이 아니라,
                    구현이 system_clock 또는 steady_clock 중 하나를 alias하는 경우가 많다.
                  - 따라서 epoch/성격도 구현에 따라 달라진다.
        */

        using namespace std::chrono;

        std::cout << "\n==================== [system_clock: time_point / epoch] ====================\n";

        // time_point: "system_clock의 epoch로부터의 오프셋"
        system_clock::time_point now_tp = system_clock::now();

        // epoch로부터 얼마나 지났는지 (duration)
        auto since_epoch = now_tp.time_since_epoch();
        auto ms_since_epoch = duration_cast<milliseconds>(since_epoch).count();

        std::cout << "system_clock::now().time_since_epoch() = " << ms_since_epoch << " ms\n";

        // system_clock는 time_t로 변환 가능 (현실 시각 출력)
        std::time_t now_tt = system_clock::to_time_t(now_tp);

        std::tm utc_tm{};
        std::tm local_tm{};

        if (!gmtime_s(&utc_tm, &now_tt))
        {
            std::cout << "gmtime_s failed\n";
            return;
        }

        if (!localtime_s(&utc_tm, &now_tt))
        {
            std::cout << "localtime_s failed\n";
            return;
        }

        std::cout << "UTC   : " << format_tm(utc_tm, "%Y-%m-%d %H:%M:%S") << "\n";
        std::cout << "Local : " << format_tm(local_tm, "%Y-%m-%d %H:%M:%S") << "\n";

        // time_point - time_point = duration
        system_clock::time_point t1 = system_clock::now();
        std::this_thread::sleep_for(milliseconds(120));
        system_clock::time_point t2 = system_clock::now();

        auto dt = duration_cast<milliseconds>(t2 - t1).count();
        std::cout << "t2 - t1 = " << dt << " ms (system_clock로 측정)\n";

        // 주의: system_clock는 시스템 시간 변경(NTP/수동 변경)으로 점프할 수 있음
        std::cout << "NOTE: system_clock는 시스템 시간 변경에 의해 점프할 수 있음(경과시간 측정에 비추). \n";

        system("pause");
    }

    //=============================================================================================

    void duration()
    {
        /*
            📚 std::chrono 핵심 3요소: duration (C++11)

              - duration : "시간 길이(얼마나) = tick * period"
                           "구간"이다. (1초, 250ms, 3분, 2시간...)
              - 절대적인 날짜/시간 의미가 없다.
              - 내부적으로는 다음 형태:

              std::chrono::duration<Rep, Period>
              Rep    : 저장 타입 (예: long long)
              Period : 1 tick이 몇 초인지 (예: std::milli = 1/1000)

              - 예:
                std::chrono::milliseconds  -> duration<long long, milli>
                std::chrono::seconds       -> duration<long long, ratio<1,1>>
                std::chrono::duration<double> -> "초 단위"를 double로 저장

              - 특징:
                ✅ duration끼리 +, -, 비교 가능
                ✅ duration_cast로 단위 변환 가능
                ✅ "길이"이므로 벽시계 점프/NTP 보정 같은 영향을 직접 받지 않는다
        */

        using namespace std::chrono;

        std::cout << "=== Duration ===\n";

        milliseconds d1(250);                // 250ms
        seconds      d2(2);                  // 2s
        minutes      d3(3);                  // 3min

        // duration_cast로 단위 변환
        auto d3_in_sec = duration_cast<seconds>(d3);

        std::cout << "d1 = " << d1.count() << " ms\n";
        std::cout << "d2 = " << d2.count() << " s\n";
        std::cout << "d3 = " << d3.count() << " min\n";
        std::cout << "d3_in_sec = " << d3_in_sec.count() << " s\n";

        // 덧셈/뺄셈도 "길이" 연산
        auto total_ms = duration_cast<milliseconds>(d2 + d1); // 2s + 250ms = 2250ms
        std::cout << "d2 + d1 = " << total_ms.count() << " ms\n\n";


        // duration끼리 연산 (단위가 다르면 공통 단위로 맞춰짐)
        auto sum = d1 + d2; // 결과 타입은 구현/템플릿 규칙에 의해 결정되지만 값은 올바르게 합산됨
        std::cout << "d1 + d2 = " << duration_cast<milliseconds>(sum).count() << " ms (ms로 캐스팅해 출력)\n";

        microseconds d4 = duration_cast<microseconds>(d1); // 250ms -> 250000us

        // double 기반 duration (소수초 표현)
        std::chrono::duration<double> sec = d1; // 250ms -> 2.5초
        std::cout << "d1 = " << sec.count() << " seconds (double)\n";

        // 1 tick이 100ns인 커스텀 duration (예시)
        using hundred_ns = std::chrono::duration<long long, std::ratio<1, 10'000'000>>; // 1/10,000,000 sec = 100ns
        hundred_ns h(12345);
        auto ns = std::chrono::duration_cast<nanoseconds>(h);
        std::cout << "hundred_ns(12345 ticks) = " << ns.count() << " ns\n";

        system("pause");
    }

    //=============================================================================================

    void sleep_wait()
    {
        using namespace std::chrono;

        std::cout << "=== Sleep / Wait ===\n";

        // (A) sleep_for: "길이(duration)만큼" 쉬기
        std::cout << "sleep_for(300ms) ...\n";
        std::this_thread::sleep_for(milliseconds(300));
        std::cout << "woke up\n\n";

        // (B) sleep_until: "특정 시각(time_point)까지" 기다리기
        auto wake_at = steady_clock::now() + milliseconds(500);
        std::cout << "sleep_until(now + 500ms) ...\n";
        std::this_thread::sleep_until(wake_at);
        std::cout << "woke up\n\n";

        // (C) 간단한 "타임아웃 루프" 예시: 2초 동안 폴링(10ms 간격)
        auto deadline = steady_clock::now() + seconds(2);

        std::cout << "polling loop for up to 2 seconds...\n";
        int tries = 0;
        while (steady_clock::now() < deadline)
        {
            ++tries;

            // 여기서 "어떤 조건"을 체크한다고 가정
            bool done = false; // 데모: 항상 false

            if (done)
            {
                std::cout << "done! tries=" << tries << "\n\n";
                return;
            }

            std::this_thread::sleep_for(milliseconds(10)); // CPU 과점유 방지
        }

        std::cout << "timeout reached. tries=" << tries << "\n\n";

        system("pause");
    }


	void Test()
	{
        sleep_wait();

        duration();

        time_point();

        high_resolution_clock_use();

        steady_clock_use();

        system_clock_use();

        std_chrono_what();
	}
}//Chrono