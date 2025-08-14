#include "stdafx.h"

#include <format>
#include <vector>
#include <span>


namespace MemorySpan
{
    void print_all(std::span<const int> arr) 
    {
        for (int v : arr)
            std::cout << v << ' ';
        std::cout << std::endl;
    }

	void span_use()
	{
        /*
            📚 std::span()

              - C++20에서 도입된 범위(View) 기반의 경량 래퍼 타입
              - 외부 배열, std::vector, std::array 등의 연속 메모리 구간을 '복사 없이' 참조하는 객체
              - 크기와 포인터만 내부적으로 관리(값 복사 X)
              - 함수에서 배열/버퍼를 받을 때, 포인터+길이 인수 대신 span을 활용해 더 안전하고 편리한 API 작성 가능
              - 읽기/쓰기 모두 지원 (`std::span<T>`) / 읽기 전용(`std::span<const T>`)
              - slicing, subspan, iteration, size(), data() 등 다양한 편의 기능 제공
              - 메모리 소유 X(단순 뷰) → 관리 대상 원본 객체의 생명주기 주의 필요
              - STL 컨테이너와 배열, 포인터 모두에서 쉽게 생성 가능
              - 헤더: <span>, C++20부터 지원
        */
        {
            int a[] = { 1, 2, 3, 4, 5 };
            std::vector<int> v = { 10, 20, 30, 40, 50 };

            // 배열, 벡터 모두에서 span 생성 가능
            std::span<int> s1(a);
            std::span<int> s2(v);

            // 슬라이싱
            auto s3 = s2.subspan(1, 3); // 20, 30, 40

            print_all(s1);  // 출력: 1 2 3 4 5
            print_all(s2);  // 출력: 10 20 30 40 50
            print_all(s3);  // 출력: 20 30 40

            system("pause");
        }
	}

	void Test()
	{
		span_use();
	}
}//MemorySpan