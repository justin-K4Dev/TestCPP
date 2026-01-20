#include "stdafx.h"

#include <Windows.h>


namespace ThreadSyncWithVolatile
{
    volatile bool g_flag = true;      // 플래그: volatile만 사용
    int g_sharedData = 0;             // 공유 데이터

    CRITICAL_SECTION g_cs;            // Windows용 동기화 객체

    unsigned __stdcall writer_with_cs(void*)
    {
        Sleep(1); // 1ms 대기
        EnterCriticalSection(&g_cs);
        g_sharedData = 42;
        g_flag = false; // 동기화 하에 플래그 변경
        LeaveCriticalSection(&g_cs);
        std::cout << "[Writer] g_sharedData set to 42, g_flag set to false\n";
        return 0;
    }

    unsigned __stdcall reader_with_cs(void*)
    {
        // 플래그가 false가 될 때까지 대기
        while (true) {
            EnterCriticalSection(&g_cs);
            if (!g_flag) {
                int val = g_sharedData;
                LeaveCriticalSection(&g_cs);
                std::cout << "[Reader] g_flag is false, g_sharedData = " << val << std::endl;
                break;
            }
            LeaveCriticalSection(&g_cs);
            Sleep(0); // CPU yield
        }
        return 0;
    }

    unsigned __stdcall writer_with_volatile(void*)
    {
        Sleep(1); // 1ms 대기
        g_sharedData = 99;
        g_flag = false; // volatile만으로 동기화 (권장X)
        std::cout << "[Writer-volatile] g_sharedData set to 99, g_flag set to false\n";
        return 0;
    }

    unsigned __stdcall reader_with_volatile(void*)
    {
        // 플래그가 false가 될 때까지 대기
        while (g_flag) {
            Sleep(0); // CPU yield
        }
        // volatile만으로는 데이터 최신성/원자성 보장 X
        std::cout << "[Reader-volatile] g_flag is false, g_sharedData = " << g_sharedData << std::endl;
        return 0;
    }

    void thread_sync_with_volatile()
    {
        /*
            📚 _beginthreadex + volatile 연동 동기화 비교 (C++98)

              - 본 예제는 _beginthreadex를 사용한 Windows 환경에서,
                1) volatile 키워드만 사용할 때의 한계
                2) CRITICAL_SECTION(mutex) 기반의 올바른 동기화 패턴을 비교 한다.

              ◈ 주요 내용
                - volatile은 "최적화 제한"만 보장하며, 멀티스레드 데이터 동기화/순서/원자성/가시성을 보장하지 못함
                - CRITICAL_SECTION 등 OS mutex를 사용할 때만 실제 동기화가 보장됨
                - spin lock, 플래그, 공유 데이터 등에서 volatile과 mutex의 차이 체험

              ◈ 실전 의의
                - C++98/C++03까지는 표준 동기화 도구가 없으므로,
                  멀티스레드 환경에서는 반드시 운영체제 동기화 객체를 사용해야 함
                - C++11 이후라면 std::mutex, std::atomic 사용 권장

              ※ build: Windows/MSVC 환경, C++98/03, /EHsc
        */

        {
            std::cout << "==== CRITICAL_SECTION 동기화 예제 ====\n";
            g_flag = true; g_sharedData = 0;
            InitializeCriticalSection(&g_cs);

            HANDLE th_writer = (HANDLE)_beginthreadex(NULL, 0, writer_with_cs, NULL, 0, NULL);
            HANDLE th_reader = (HANDLE)_beginthreadex(NULL, 0, reader_with_cs, NULL, 0, NULL);

            WaitForSingleObject(th_writer, INFINITE);
            WaitForSingleObject(th_reader, INFINITE);

            CloseHandle(th_writer);
            CloseHandle(th_reader);

            DeleteCriticalSection(&g_cs);

            std::cout << "\n==== volatile-only 예제 (권장X) ====\n";
            g_flag = true; g_sharedData = 0;

            HANDLE th_writer2 = (HANDLE)_beginthreadex(NULL, 0, writer_with_volatile, NULL, 0, NULL);
            HANDLE th_reader2 = (HANDLE)_beginthreadex(NULL, 0, reader_with_volatile, NULL, 0, NULL);

            WaitForSingleObject(th_writer2, INFINITE);
            WaitForSingleObject(th_reader2, INFINITE);

            CloseHandle(th_writer2);
            CloseHandle(th_reader2);

            system("pause");
        }

    }

    void Test()
    {
        thread_sync_with_volatile();
    }
}