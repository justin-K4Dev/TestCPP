#include "stdafx.h"

#define BUILDING_DLL
#include "dll-implicit-api.h"

extern "C" DLL_API int imp_add(int a, int b) { return a + b; }
extern "C" DLL_API int imp_sub(int a, int b) { return a - b; }

extern "C" DLL_API int imp_widget_sum_range(int start, int count)
{
    auto v = LibCpp::make_range(start, count);
    LibCpp::Widget w("imp");
    for (int x : v) w.add_value(x);
    return w.sum();
}

extern "C" DLL_API void imp_abi_get_info(AbiInfo* outInfo)
{
    fill_abi_info(outInfo);
}

extern "C" DLL_API int32_t imp_abi_check_compat(const AbiExpected* exp)
{
    AbiInfo me{};
    fill_abi_info(&me);
    return check_abi_compat(exp, &me);
}

// ---- STL-safe wrappers (imp_) ----
extern "C" DLL_API StlHandle* imp_stl_create(uint32_t seed) { return stl_core_create(seed); }
extern "C" DLL_API void       imp_stl_destroy(StlHandle* h) { stl_core_destroy(h); }

extern "C" DLL_API int  imp_stl_set_name(StlHandle* h, const char* s) { return stl_core_set_name(h, s); }
extern "C" DLL_API int  imp_stl_get_name(StlHandle* h, char* outBuf, size_t outBytes, size_t* need)
{
    return stl_core_get_name(h, outBuf, outBytes, need);
}

extern "C" DLL_API char* imp_stl_alloc_name(StlHandle* h) { return stl_core_alloc_name(h); }
extern "C" DLL_API void  imp_stl_free(char* p) { stl_core_free(p); }

extern "C" DLL_API int     imp_stl_push(StlHandle* h, int v) { return stl_core_push(h, v); }
extern "C" DLL_API size_t  imp_stl_count(StlHandle* h) { return stl_core_count(h); }
extern "C" DLL_API int     imp_stl_get_values(StlHandle* h, int* outArr, size_t outCount, size_t* need)
{
    return stl_core_get_values(h, outArr, outCount, need);
}
extern "C" DLL_API int     imp_stl_sum(StlHandle* h) { return stl_core_sum(h); }
