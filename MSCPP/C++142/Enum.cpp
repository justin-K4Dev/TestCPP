#include "stdafx.h"


#include <vector>
#include <iostream>

namespace Enum
{
    void do_fallthrough(int n)
    {
        switch (n) {
        case 1:
            std::cout << "case 1" << std::endl;
            // 의도적으로 fall through
            [[fallthrough]]; // C++17~: fallthrough attribute

        case 2:
            std::cout << "case 2" << std::endl;
            break;

        case 3:
            std::cout << "case 3" << std::endl;
            // 의도적 fall-through 표시 안하면, 일부 컴파일러에서 경고 가능
            // (하지만 C++17 이상에선 명확하게 쓰길 권장)

        default:
            std::cout << "default" << std::endl;
            break;
        }
    }

    void fallthrough_use()
    {
        /*
            📚 [[fallthrough]] attribute 개요 (C++17~)

              - C++17에서 도입된 표준 속성(attribute) 중 하나
              - switch 문에서 의도적으로 "case 문 사이에 break 없이 다음 case로 넘어감(=fall-through)"을
                명확하게 표시하는 용도
              - [[fallthrough]]를 쓰면, 컴파일러가 "break 누락" 경고를 내지 않음 (일부 컴파일러는 fall-through에 대해 경고함)
              - 오타 등으로 실수로 break를 빼먹었는지, 정말 의도적으로 썼는지 구분 가능
              - 반드시 "null statement" (즉, ; 한 줄)에 붙여야 함
              - 여러 case 사이에서 사용 가능
              - 헤더 불필요, /std:c++17 이상 필요 (MSVC, GCC, Clang 지원)
              - 자세한 내용: http://en.cppreference.com/w/cpp/language/attributes
        */

        do_fallthrough(1);
        do_fallthrough(2);
        do_fallthrough(3);
    }

    void Test()
    {
        fallthrough_use();
    }

}//end of namespace