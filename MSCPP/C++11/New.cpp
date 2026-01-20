#include "stdafx.h"



// 전역 할당 트래킹 컨테이너
std::vector<void*> myAlloc;

// 실제 메모리 할당 및 로깅 함수
void* newImpl(std::size_t sz, char const* file, int line)
{
    void* ptr = std::malloc(sz);
    std::cerr << file << ": " << line << " " << ptr << std::endl;
    myAlloc.push_back(ptr);
    return ptr;
}

// new 연산자 재정의: 파일명, 라인 넘버 받기
void* operator new(std::size_t sz, char const* file, int line)
{
    return newImpl(sz, file, line);
}

// new[] 연산자 재정의 (배열 할당)
void* operator new[](std::size_t sz, char const* file, int line)
{
    return newImpl(sz, file, line);
}

// delete 연산자 재정의: 추적 목록에서 제거 및 실제 메모리 해제
void operator delete(void* ptr) noexcept
{
    auto it = std::find(myAlloc.begin(), myAlloc.end(), ptr);
    if (it != myAlloc.end()) {
        *it = nullptr;
    }
    std::free(ptr);
}

// delete[] 연산자도 필요하다면 동일하게 재정의 가능
// new 호출 시 자동으로 파일/라인 정보 삽입
#define new new(__FILE__, __LINE__)


namespace New
{
    // 테스트용 함수: int 1개 동적 할당 및 메모리 주소 확인
    void dummyFunction()
    {
        int* dummy = new int;
        *dummy = 42; // 사용 예시

        /*
        출력:
            New.cpp: 54 0000000000ABCDEF
        */
        delete dummy; // 명시적으로 해제

        system("pause");
    }

    // 할당된 메모리 추적 정보 출력
    void getInfo()
    {
        std::cout << "Allocation: " << std::endl;
        for (auto& i : myAlloc) {
            if (i != nullptr) {
                std::cout << " " << i << std::endl;
            }
        }
        /*
        출력:
            Allocation:
             0000000000ABCDEF
        */

        system("pause");
    }

    void new_overloading()
    {
        /*
            📚 new 연산자 재정의(Overloading new/delete Operators)

              - C++에서는 전역 또는 클래스 단위로 new/delete, new[]/delete[] 연산자를 재정의할 수 있음.
              - 주요 목적:
                • 커스텀 메모리 할당 로직(예: 메모리 풀, 로깅, 트래킹, 디버깅 지원 등)
                • 파일/라인 등 디버깅 정보와 연계
                • 메모리 릭(누수) 검사, 해제 타이밍 추적
              - 흔한 활용법: new 연산자 호출 시 파일명/라인 정보를 받아 메모리 위치 기록 및 추적

              - 주의: new, delete 연산자 재정의는 프로그램 전체 메모리 관리에 영향을 줄 수 있으므로
                정확한 해제/추적 로직, 예외 안정성, 다중 스레드 환경 등에서 꼼꼼하게 설계해야 함.


            💡 주요 테스트 포인트
              - new int 호출시 할당 주소와 소스 파일/라인 정보가 stderr로 출력됨
              - 할당된 포인터는 myAlloc에 저장되고, delete 호출시 nullptr로 처리됨
              - getInfo() 호출시 아직 해제되지 않은(누수된) 포인터만 출력됨
              - 실전에서는 delete[], 멀티스레드 환경, 예외 안전성, double-free 체크 등도 반드시 보완해야 함
        */
    }

    void Test()
    {
        dummyFunction();

        getInfo();

        new_overloading();
    }
}