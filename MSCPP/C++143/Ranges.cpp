#include "stdafx.h"

#include <iostream>
#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
#include <tuple>
#include <utility>


namespace Ranges
{
    void Ranges_what()
    {
        /*
            📚 <ranges> (C++20)

              - C++20에서 새롭게 도입된 **범위 기반(range-based) 알고리즘/뷰(view) 시스템**
              - 기존의 `<algorithm>`, `<iterator>` 기반 STL보다 더 **표현력 있고 안전한 데이터 흐름 처리** 가능
              - "범위(range)"는 [begin, end) 쌍을 나타내는 컨테이너 또는 뷰를 의미함

              🔹 주요 특징
                - 모든 컨테이너를 직접 조작하지 않고 **뷰(view)** 를 통해 지연 평가 방식 처리
                - 체이닝 가능 (파이프 스타일: `|`)
                - `std::ranges::algo()` → 범위 인자 기반 알고리즘
                - `std::views::xxx`     → 변형 가능한 지연 평가 어댑터

              🔹 주요 구성
                1. **Views (뷰 어댑터)**:
                  - `std::views::filter`     → 조건에 맞는 값만 필터링
                  - `std::views::transform`  → 각 요소에 함수 적용
                  - `std::views::take(n)`    → 앞에서 n개만 취함
                  - `std::views::drop(n)`    → 앞에서 n개 버림
                  - `std::views::reverse`, `join`, `split`, `enumerate` 등

                2. **Algorithms (알고리즘)**:
                  - `std::ranges::sort`, `find`, `copy`, `for_each`, `count`, `equal` 등
                  - 기존 `<algorithm>`과 유사하지만, iterator 대신 range 자체를 사용

              🔹 도입 배경
                - 기존 STL은 반복자 조작이 복잡하고 안전하지 않음
                - Range 기반 STL은 **더 안전하고 직관적인 함수형 스타일** 제공
                - 데이터 흐름 파이프라인 구성이 자연스러움

              🔹 표준 지원
                - C++20부터 `<ranges>` 헤더 사용 가능
                - Visual Studio 2019 16.10+, GCC 10+, Clang 12+ 이상에서 대부분 지원

              🔹 예제 문법

                std::ranges::sort(vec); // 기존 std::sort보다 안전
                auto view = vec | std::views::filter(...) | std::views::transform(...);

        */
        {
            std::vector<int> vec = { 1, 2, 3, 4, 5, 6 };

            std::cout << "[original] ";
            for (int x : vec) std::cout << x << " ";
            std::cout << "\n";

            // filter: 짝수만 남기기
            auto even = [](int x) { return x % 2 == 0; };

            // transform: 제곱
            auto square = [](int x) { return x * x; };

            // 뷰 체이닝
            auto view = vec
                | std::views::filter(even)
                | std::views::transform(square);

            std::cout << "[filtered & squared] ";
            for (int x : view) std::cout << x << " ";
            std::cout << "\n";

            // 슬라이싱
            std::cout << "[first 3 elements] ";
            for (int x : vec | std::views::take(3)) std::cout << x << " ";
            std::cout << "\n";

            // ranges::sort (기존 std::sort는 iterator 필요)
            std::vector<int> unsorted = { 5, 2, 1, 4, 3 };
            std::ranges::sort(unsorted);

            std::cout << "[sorted] ";
            for (int x : unsorted) std::cout << x << " ";
            std::cout << "\n";

            /*
            출력:
                [original] 1 2 3 4 5 6
                [filtered & squared] 4 16 36
                [first 3 elements] 1 2 3
                [sorted] 1 2 3 4 5
            */
        }

        system("pause");
    }

    //=============================================================================================

    // enumerate view (커스텀 정의)
    template <std::ranges::input_range T>
    class enumerate_view : public std::ranges::view_interface<enumerate_view<T>>
    {
        T base_;
    public:
        explicit enumerate_view(T base) 
            : base_(std::move(base)) {}

        struct iterator 
        {
            std::ranges::iterator_t<T> current;
            std::size_t index = 0;

            auto operator*() const {
                return std::pair(index, *current);
            }

            iterator& operator++() {
                ++current;
                ++index;
                return *this;
            }

            bool operator==(const iterator& other) const {
                return current == other.current;
            }
        };

        auto begin() 
        {
            return iterator{ std::ranges::begin(base_), 0 };
        }

        auto end() 
        {
            return iterator{ std::ranges::end(base_), 0 };
        }
    };

    // 도우미 함수
    template <std::ranges::input_range T>
    enumerate_view(T&&) -> enumerate_view<std::views::all_t<T>>;

    void split_join_enumerate_use()
    {
        /*
            📚 views::split, views::join, enumerate
        
              🔹 views::split
                - 시퀀스를 특정 값/시퀀스로 분리 → 하위 range들의 range 생성
                - 예: 공백 또는 구분자 문자 기준 문자열 분리

              🔹 views::join
                - 중첩 range들을 펼쳐(flatten)서 하나의 range로 연결
                - split의 결과를 다시 평면화할 때 주로 사용

              🔹 enumerate (직접 구현 필요)
                - Python의 enumerate처럼 (index, value) 쌍을 제공
                - C++ 표준에는 없음 → 커스텀 view로 구현 가능
        */
        {
            using namespace std;
            using namespace std::views;

            // ✅ views::split 예제
            string text = "apple orange banana";
            auto split_view = text | std::views::split(' '); // 공백 기준으로 나눔

            cout << "[split view]\n";
            for (auto&& word : split_view) 
            {
                for (char c : word) cout << c;
                cout << "\n";
            }

            // ✅ views::join 예제
            vector<string> words = { "C++", "20", "ranges" };
            auto joined = words | std::views::transform([](const string& s) {
                return s | all;
            }) | join;

            cout << "[joined view]\n";
            for (char c : joined) cout << c; // 출력: C++20ranges
            cout << "\n";

            // ✅ enumerate 예제 (custom)
            vector<string> names = { "Alice", "Bob", "Charlie" };
            cout << "[enumerate view]\n";
            for (auto [i, name] : Ranges::enumerate_view(names))
            {
                cout << i << ": " << name << "\n";
            }
        }

        system("pause");
    }

    void Test()
    {
        split_join_enumerate_use();

        Ranges_what();
    }
}//Ranges