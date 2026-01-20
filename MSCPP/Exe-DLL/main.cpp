#include "stdafx.h"


#include <Windows.h>

#include "abi_common.h"
#include "stl_core.h"
#include "dll-implicit-api.h"
#include "dll-explicit-api.h"
#include "dll-delay-api.h"


static const char* abi_rc_to_str(int32_t rc)
{
    switch (rc) {
    case ABI_OK: return "ABI_OK";
    case ABI_ERR_NULL: return "ABI_ERR_NULL";
    case ABI_ERR_MAJOR_MISMATCH: return "ABI_ERR_MAJOR_MISMATCH";
    case ABI_ERR_MINOR_TOO_OLD: return "ABI_ERR_MINOR_TOO_OLD";
    case ABI_ERR_ITERATOR_DEBUG_MISMATCH: return "ABI_ERR_ITERATOR_DEBUG_MISMATCH";
    case ABI_ERR_CRT_FLAGS_MISMATCH: return "ABI_ERR_CRT_FLAGS_MISMATCH";
    case ABI_ERR_ARCH_MISMATCH: return "ABI_ERR_ARCH_MISMATCH";
    default: return "ABI_ERR_UNKNOWN";
    }
}

static void print_abi(const char* tag, const AbiInfo& a)
{
    std::cout << tag
        << " abi=" << a.abi_major << "." << a.abi_minor
        << " crt_flags=" << a.crt_flags
        << " itdbg=" << a.iterator_debug_level
        << " msvc=" << a.msvc_ver
        << " x64=" << a.is_64bit
        << "\n";
}

static bool LoadPlugin(const wchar_t* dllName, PluginApi& outApi, HMODULE& outMod)
{
    outMod = LoadLibraryW(dllName);
    if (!outMod) return false;

    auto get_api = (plugin_get_api_fn)GetProcAddress(outMod, "plugin_get_api");
    if (!get_api) { FreeLibrary(outMod); outMod = nullptr; return false; }

    PluginApi api{};
    if (!get_api(&api)) { FreeLibrary(outMod); outMod = nullptr; return false; }

    outApi = api;
    return true;
}

static void TestStl(const char* tag,
    StlHandle* (*create_fn)(uint32_t),
    void (*destroy_fn)(StlHandle*),
    int (*set_name_fn)(StlHandle*, const char*),
    int (*get_name_fn)(StlHandle*, char*, size_t, size_t*),
    int (*push_fn)(StlHandle*, int),
    size_t(*count_fn)(StlHandle*),
    int (*get_values_fn)(StlHandle*, int*, size_t, size_t*),
    int (*sum_fn)(StlHandle*))
{
    std::cout << "\n[" << tag << "] STL-safe\n";

    StlHandle* h = create_fn(777);
    set_name_fn(h, "Justin");

    size_t needBytes = 0;
    get_name_fn(h, nullptr, 0, &needBytes);
    std::string name(needBytes, '\0');
    get_name_fn(h, (char*)name.data(), name.size(), nullptr);

    std::cout << "  name=" << name.c_str() << "\n";

    push_fn(h, 10);
    push_fn(h, 20);
    push_fn(h, 30);

    std::cout << "  count=" << count_fn(h) << " sum=" << sum_fn(h) << "\n";

    size_t needCount = 0;
    get_values_fn(h, nullptr, 0, &needCount);
    std::vector<int> v(needCount);
    get_values_fn(h, v.data(), v.size(), nullptr);

    std::cout << "  values:";
    for (int x : v) std::cout << " " << x;
    std::cout << "\n";

    destroy_fn(h);
}

int main()
{
#if defined(_DEBUG)
    std::cout << "Exe-DLL DEBUG\n";
#else
    std::cout << "Exe-DLL RELEASE\n";
#endif

    std::cout << "Exe crt_flags=" << abi_crt_flags()
        << " itdbg=" << abi_iterator_debug_level() << "\n";

    AbiExpected exp{};
    exp.expect_abi_major = ABI_MAJOR;
    exp.min_abi_minor = ABI_MINOR;
    exp.expect_iterator_debug = abi_iterator_debug_level();
    exp.require_same_crt_flags = 0;           // C ABI 안전 설계면 보통 0
    exp.expected_crt_flags = abi_crt_flags(); // require_same_crt_flags=1일 때만 의미

    // ---------------- [1] Implicit ----------------
    std::cout << "\n[1] Implicit\n";
    {
        AbiInfo info{};
        imp_abi_get_info(&info);
        print_abi("  [imp] ", info);

        int32_t rc = imp_abi_check_compat(&exp);
        std::cout << "  [imp] abi_check=" << abi_rc_to_str(rc) << "\n";

        std::cout << "  imp_add(2,3)=" << imp_add(2, 3) << "\n";
        std::cout << "  imp_sub(9,4)=" << imp_sub(9, 4) << "\n";
        std::cout << "  imp_widget_sum_range(1,5)=" << imp_widget_sum_range(1, 5) << "\n";

        TestStl("Implicit",
            &imp_stl_create, &imp_stl_destroy, &imp_stl_set_name, &imp_stl_get_name,
            &imp_stl_push, &imp_stl_count, &imp_stl_get_values, &imp_stl_sum);
    }

    // ---------------- [2] Delay-load ----------------
    std::cout << "\n[2] Delay-load\n";
    {
        // Delay-load는 첫 호출 때 DLL 로드됨(링커 옵션)
        AbiInfo info{};
        delay_abi_get_info(&info);
        print_abi("  [delay] ", info);

        int32_t rc = delay_abi_check_compat(&exp);
        std::cout << "  [delay] abi_check=" << abi_rc_to_str(rc) << "\n";

        std::cout << "  delay_add(2,3)=" << delay_add(2, 3) << "\n";
        std::cout << "  delay_sub(9,4)=" << delay_sub(9, 4) << "\n";
        std::cout << "  delay_widget_sum_range(1,5)=" << delay_widget_sum_range(1, 5) << "\n";

        TestStl("Delay",
            &delay_stl_create, &delay_stl_destroy, &delay_stl_set_name, &delay_stl_get_name,
            &delay_stl_push, &delay_stl_count, &delay_stl_get_values, &delay_stl_sum);
    }

    // ---------------- [3] Explicit plugin ----------------
    std::cout << "\n[3] Explicit (plugin)\n";
    {
        PluginApi api{};
        HMODULE h = nullptr;
        if (!LoadPlugin(L"Lib-DLL-Explicit_d64.dll", api, h))
        {
            std::cout << "  plugin load failed\n";
        }
        else
        {
            print_abi("  [plugin] ", api.abi);

            int32_t rc = api.abi_check_compat ? api.abi_check_compat(&exp) : ABI_ERR_NULL;
            std::cout << "  [plugin] abi_check=" << abi_rc_to_str(rc) << "\n";

            std::cout << "  plugin add(2,3)=" << api.add(2, 3) << "\n";
            std::cout << "  plugin sub(9,4)=" << api.sub(9, 4) << "\n";
            std::cout << "  plugin widget_sum_range(1,5)=" << api.widget_sum_range(1, 5) << "\n";

            TestStl("Plugin",
                api.stl_create, api.stl_destroy, api.stl_set_name, api.stl_get_name,
                api.stl_push, api.stl_count, api.stl_get_values, api.stl_sum);

            FreeLibrary(h);
        }
    }

    std::cout << "\nDONE\n";
    return 0;
}