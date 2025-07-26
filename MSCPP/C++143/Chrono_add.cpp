#include "stdafx.h"

#include <iostream>
#include <chrono>
#include <format>


namespace Chrono_AddFeatures
{
    void Chrono_AddFeatures()
    {
        /*
            📚 <chrono> 추가 기능 (C++20)

              - C++20에서 `<chrono>` 라이브러리에 **캘린더 날짜 처리 및 시간대 지원** 기능이 추가됨
              - 기존의 duration/clock 위주의 `<chrono>`를 넘어, **사람 친화적인 날짜/시간 표현**이 가능해짐

              🔹 주요 추가 요소

                1. 날짜 캘린더 시스템:
                  - `std::chrono::year`
                  - `std::chrono::month`
                  - `std::chrono::day`
                  - `std::chrono::year_month_day` (복합 날짜 타입)
                  - `std::chrono::weekday`, `month_day`, `year_month`

                2. 날짜 연산:
                  - `std::chrono::sys_days` → `time_point`와 `calendar` 상호 변환
                  - 연/월/일 단위 덧셈/비교 연산 가능
                  - `std::chrono::floor<std::chrono::days>(system_clock::now())`

                3. 시간대(zones) 지원:
                  - `std::chrono::zoned_time<Clock>`: 타임존을 포함한 시간 표현
                  - `std::chrono::current_zone()`: 현재 시스템 시간대
                  - `std::chrono::locate_zone("Asia/Seoul")`

                4. 문자열 포맷 출력 (`std::format`)
                  - `%Y-%m-%d`, `%H:%M:%S` 등 strftime 스타일 포맷 사용 가능
                  - `std::format("{:%F}", some_day)` → ISO 날짜 문자열

              🔹 장점
                - std::tm / time_t / localtime 등 구식 C 인터페이스를 대체
                - 지역화, 표준화된 시간 연산을 안전하고 직관적으로 수행 가능
                - 시간대 인식 애플리케이션(서버, 클라이언트, UI)에 적합

              🔹 표준 지원
                - C++20 이상
                - 시간대(zone) 기능은 OS + 라이브러리에서 **시간대 데이터(tzdb)** 필요
                - Visual Studio 2022, GCC 11+, Clang 14+ 일부 제한 지원

              🔹 예제 문법

                std::chrono::year_month_day ymd = std::chrono::floor<std::chrono::days>(system_clock::now());
                std::cout << std::format("{:%Y-%m-%d}", ymd); // 현재 날짜 출력

                auto zt = std::chrono::zoned_time(std::chrono::current_zone(), system_clock::now());
                std::cout << std::format("{:%Y-%m-%d %H:%M:%S %Z}", zt); // 타임존 포함 출력
        */
        {
            using namespace std;
            using namespace std::chrono;

            // 현재 날짜: system_clock -> days -> year_month_day 변환
            auto today_tp = floor<days>(system_clock::now());
            year_month_day ymd = today_tp;

            cout << "[현재 날짜]\n";
            cout << format("{:%Y-%m-%d}", ymd) << "\n"; // ISO 8601 형식

            // 날짜 연산: 다음 달
            auto next_month = ymd.month() + months{ 1 };
            year_month_day next_ymd = year_month_day{ ymd.year(), next_month, ymd.day() };

            cout << "[다음 달 같은 날]\n";
            cout << format("{:%Y-%m-%d}", next_ymd) << "\n";

            // 시간대 사용 (zoned_time)
            try {
                auto now = system_clock::now();
                zoned_time zt(current_zone(), now);

                cout << "[현재 시간 + 시간대]\n";
                cout << format("{:%Y-%m-%d %H:%M:%S %Z}", zt) << "\n";
            }
            catch (const std::exception& e) {
                cout << "[zoned_time 예외] " << e.what() << "\n";
            }
        }

        system("pause");
    }

    void differ_timezeon_time()
    {
        using namespace std;
        using namespace std::chrono;

        auto tz = current_zone();
        zoned_time local_time{ tz, system_clock::now() };

        auto ny_zone = locate_zone("America/New_York");
        zoned_time ny_time{ ny_zone, local_time.get_sys_time() };

        auto duration = local_time.get_sys_time() - ny_time.get_sys_time();
        cout << "Time difference: " << duration_cast<hours>(duration).count() << " hours\n";

        system("pause");
    }

    void other_timezone_time()
    {
        using namespace std;
        using namespace std::chrono;

        auto tz = current_zone();
        zoned_time zt{ tz, system_clock::now() };

        auto ny_zone = locate_zone("America/New_York");

        // 다른 시간대로 변환
        zoned_time ny_time{ ny_zone, zt.get_sys_time() };
        cout << "New York time: " << format("{:%F %T %Z}", ny_time) << endl;

        system("pause");
    }


    void Test()
    {
        other_timezone_time();

        differ_timezeon_time();

        Chrono_AddFeatures();
    }
}//Chrono_AddFeatures