#include "stdafx.h"


#include <string_view>
#include <optional>


namespace STL_Improvements
{
    void STL_improvements()
    {
        /*
            📚 Visual C++17/MSVC 최신 STL(표준 라이브러리) 주요 개선사항

              - std::string, std::vector 등 기본 컨테이너 성능·진단·표준 준수 대폭 향상
                • string iterator 진단 메시지 개선 (디버그 시 오류 원인 더 상세히 표시)
                • basic_string::find(char): 길이 1짜리 일반 검색에서 traits::find 1회 호출로 최적화
                • operator==: 길이 먼저 비교(짧으면 바로 return)
                • reserve() 불필요 작업 감소, 최적화
                • string move 생성자 성능 3배 향상

              - <any>, <string_view>, <optional>, <variant>, <cstdalign> 등 최신 헤더 추가
              - user-defined allocator, propagate_on_container_* 연산 관련 코드젠 버그 수정
              - 자세한 내용은 "STL Fixes In VS 2017 RTM" 공식 문서 참고
        */
        {
            // string_view 사용 (C++17~)
            std::string_view sv = "Hello, world!";
            std::cout << "string_view: " << sv.substr(7, 5) << std::endl; // 출력: world

            // optional 사용
            std::optional<int> op1;
            op1 = 10;
            if (op1) std::cout << "optional: " << *op1 << std::endl; // 출력: 10

            // variant 사용
            std::variant<int, std::string> v;
            v = 42;
            std::cout << "variant<int>: " << std::get<int>(v) << std::endl; // 출력: 42
            v = std::string("abc");
            std::cout << "variant<string>: " << std::get<std::string>(v) << std::endl; // 출력: abc

            // any 사용
            std::any val = 3.14;
            std::cout << "any<double>: " << std::any_cast<double>(val) << std::endl; // 출력: 3.14

            /*
                출력:
                string_view: world
                optional: 10
                variant<int>: 42
                variant<string>: abc
            */
        }
    }

    void Test()
    {
        STL_improvements();
    }

}//STL_Improvements