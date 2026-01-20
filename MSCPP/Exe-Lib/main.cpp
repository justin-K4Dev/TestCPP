#include "stdafx.h"

#include "lib-c-api.h"
#include "lib-cpp-api.h"


static void print_sep(const char* title)
{
    std::cout << "\n====================\n" << title << "\n====================\n";
}

static void print_vec(const std::vector<int>& v)
{
    std::cout << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]\n";
}

int main()
{
    std::cout << "Exe-Lib\n";

    // Lib-C-Static lib 관련 연동 코드
    {
        print_sep("[1] Lib-C-Static : Safe pattern (create/destroy in same lib)");
        {
            char* msg = c_make_message("Justin");
            std::cout << "msg: " << (msg ? msg : "(null)") << "\n";
            c_destroy_message(msg);
        }

        print_sep("[2] Lib-C-Static : Unsafe pattern (lib alloc -> exe free) -- for experiment");
        {
            // lib에서 malloc
            void* p = c_alloc(64);
            std::cout << "allocated p=" << p << "\n";

            // ❌ 위험: Exe-Lib의 CRT 링크가 Lib-C-Static과 일치 하지 않으면 std::free()로 해제시 오류가 발생할 수 있음 !!!
            // (빌드가 강제로 통과한 경우에만, 런타임 문제가 재현될 수 있음)
            std::free(p);

            std::cout << "freed by exe free()\n";
        }

        print_sep("[3] Lib-C-Static : 일반 함수 호출 !!!");
        {
            std::cout << "c_sum(2,3)=" << c_sum(2, 3) << "\n";
        }

        system("pause");
    }

    // Lib-CPP-Static lib 관련 연동 코드
    {
        print_sep("[1] Lib-CPP-Static : Widget object 생성 및 사용 !!!");
        {
            LibCpp::Widget w("alpha");
            w.add_value(10);
            w.add_value(20);
            std::cout << "name=" << w.name() << " sum=" << w.sum() << "\n";
            print_vec(w.values());

            auto r = LibCpp::make_range(100, 5);
            print_vec(r);

            try {
                w.require_min_size(5); // 일부러 실패
            }
            catch (const std::exception& e) {
                std::cout << "caught: " << e.what() << "\n";
            }
        }

        system("pause");
    }

    return 0;
}