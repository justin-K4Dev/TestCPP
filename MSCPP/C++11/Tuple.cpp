#include "stdafx.h"


#include <tuple>


namespace Tuple
{
    void std_tuple_what()
    {
        /*
            📚 std::tuple (C++11)

              - 서로 다른 타입의 여러 값을 하나의 묶음으로 저장하는 고정 크기 컨테이너
              - std::pair의 일반화된 형태 (2개 이상의 항목 지원)
              - 템플릿 기반으로 구성되며, 값의 개수와 타입을 컴파일 타임에 지정

              - 주요 사용 목적:
                ✅ 서로 다른 타입들을 그룹화하여 반환 또는 저장
                ✅ 다중 리턴 함수
                ✅ 구조적 바인딩(C++17), `std::tie`, `std::get` 등을 활용한 분해

              🔹 핵심 구성 요소:
                - std::tuple<T1, T2, ...>     : 튜플 선언
                - std::make_tuple(...)        : 타입 추론 기반 생성
                - std::get<I>(tuple)          : 인덱스로 접근
                - std::tie(a, b, ...)         : 변수에 분해 대입
                - std::tuple_size, tuple_element : 타입 정보 조회
        */
        {
            // 튜플 생성
            std::tuple<int, std::string, double> t = std::make_tuple(100, "C++", 3.14);

            // 튜플 요소 접근
            int id = std::get<0>(t);
            std::string name = std::get<1>(t);
            double pi = std::get<2>(t);

            // 출력
            std::cout << "ID    : " << id << "\n";
            std::cout << "Name  : " << name << "\n";
            std::cout << "Value : " << pi << "\n";

            /*
            출력:
                ID    : 100
                Name  : C++
                Value : 3.14
            */

            system("pause");
        }
    }

    //=============================================================================================

    void std_tie_use()
    {
        /*
            std::tie
        */

        // 변수 분해 (역방향 unpacking)
        {
            int id;
            std::string name;
            double score;

            std::tie(id, name, score) = std::make_tuple(42, "Alice", 99.9);

            std::cout << "id    : " << id << "\n";
            std::cout << "name  : " << name << "\n";
            std::cout << "score : " << score << "\n";
        }

        system("pause");
    }

    //=============================================================================================

    void Tuple_size_and_element()
    {
        /*
            std::tuple_size, std::tuple_element        
        */
        {
            using MyTuple = std::tuple<int, double, char>;

            constexpr size_t size = std::tuple_size<MyTuple>::value;
            std::cout << "Tuple size: " << size << "\n";

            // 타입 추론 (컴파일 타임 타입)
            using SecondType = std::tuple_element<1, MyTuple>::type;
            std::cout << "Second type is double? " << std::is_same<SecondType, double>::value << "\n";
        }

        system("pause");
    }

    //=============================================================================================

    void Tuple_compare()
    {
        /*
            Tuple 비교 (==, !=, <, >)
        */
        {
            std::tuple<int, char> a = std::make_tuple(1, 'a');
            std::tuple<int, char> b = std::make_tuple(1, 'b');

            std::cout << std::boolalpha;
            std::cout << "a == b : " << (a == b) << "\n";
            std::cout << "a <  b : " << (a < b) << "\n";
        }

        system("pause");
    }

    //=============================================================================================

    void Tuple_overlap()
    {
        /*
            Tuple 중첩 : Tuple 내에 Tuple
        */
        {
            auto inner = std::make_tuple(1, 2);
            auto outer = std::make_tuple("outer", inner);

            std::cout << "Outer string : " << std::get<0>(outer) << "\n";
            std::cout << "Inner first  : " << std::get<0>(std::get<1>(outer)) << "\n";
            std::cout << "Inner second : " << std::get<1>(std::get<1>(outer)) << "\n";
        }

        system("pause");
    }

    //=============================================================================================

    std::tuple<int, std::string> getUser() 
    {
        return std::make_tuple(1001, "Bob");
    }

    void Tuple_return_with_tie()
    {
        /*
            Tuple 반환 함수 + std::tie 활용
        */
        {
            int userId;
            std::string name;
            std::tie(userId, name) = getUser();

            std::cout << "User ID: " << userId << ", Name: " << name << "\n";
        }

        system("pause");
    }

    //=============================================================================================

    void Tuple_with_ignore()
    {
        /*
            Tuple + std::ignore 활용 : 일부 속성을 무시 !!!
        */
        {
            std::tuple<int, std::string, double> t = std::make_tuple(1, "SkipMe", 3.14);

            int id;
            double val;
            std::tie(id, std::ignore, val) = t;

            std::cout << "id  : " << id << "\n";
            std::cout << "val : " << val << "\n";
        }

        system("pause");
    }

    //=============================================================================================

    template <typename T>
    std::tuple<T, T, T> triple(T a, T b, T c) {
        return std::make_tuple(a, b, c);
    }

    template <typename T>
    std::tuple<T, T> two(T a, T b) {
        return std::make_tuple(a, b);
    }

    template <typename T>
    std::tuple<T, T> minMax(T a, T b) 
    {
        return (a < b) ? std::make_tuple(a, b) : std::make_tuple(b, a);
    };

    // 사용자 정의 타입 예제
    struct Player
    {
        std::string name;
        int score;

        bool operator<(const Player& other) const
        {
            return score < other.score;
        }

        friend std::ostream& operator<<(std::ostream& os, const Player& p)
        {
            return os << p.name << "(" << p.score << ")";
        }
    };

    void Tuple_return_with_multiple_values()
    {
        /*
            📚 템플릿 함수에서 Tuple로 여러 값 반환

              - 다양한 타입과 개수를 가진 값을 묶어서 안전하게 반환할 수 있음
              - 개수 제한 없음 (2개, 3개 이상도 가능)
              - 의미상 묶음이 필요하면 중첩 tuple도 사용 가능
        */
        {
            // 기본 int min/max 반환
            auto result = minMax(42, 17);
            std::cout << "[minMax<int>]       : Min = " << std::get<0>(result)
                << ", Max = " << std::get<1>(result) << "\n";

            // 문자열 min/max
            auto sResult = minMax(std::string("zoo"), std::string("apple"));
            std::cout << "[minMax<string>]    : Min = " << std::get<0>(sResult)
                << ", Max = " << std::get<1>(sResult) << "\n";

            // 사용자 정의 타입
            auto pResult = minMax(Player{ "Alice", 95 }, Player{ "Bob", 87 });
            std::cout << "[minMax<Player>]    : Min = " << std::get<0>(pResult)
                << ", Max = " << std::get<1>(pResult) << "\n";

            // triple 사용
            auto tResult = triple(1, 2, 3);
            std::cout << "[triple<int>]       : " << std::get<0>(tResult)
                << ", " << std::get<1>(tResult)
                << ", " << std::get<2>(tResult) << "\n";

            // two 사용 예제
            auto t2 = two(100, 200);
            std::cout << "[two<int>]          : a = " << std::get<0>(t2)
                << ", b = " << std::get<1>(t2) << "\n";

            // std::tie 로 분해
            int a, b;
            std::tie(a, b) = two(100, 200);
            std::cout << "[std::tie]          : a = " << a << ", b = " << b << "\n";

            // 중첩 튜플 예제
            auto nested = std::make_tuple("user1", std::make_tuple(300, 400));
            std::cout << "[nested tuple]      : " << std::get<0>(nested)
                << " (HP=" << std::get<0>(std::get<1>(nested))
                << ", MP=" << std::get<1>(std::get<1>(nested)) << ")\n";
        }

        system("pause");
    }


    void Test()
    {
        Tuple_return_with_multiple_values();

        Tuple_with_ignore();

        Tuple_return_with_tie();

        Tuple_size_and_element();

        std_tie_use();

        std_tuple_what();
    }
}//Tuple