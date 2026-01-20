#pragma once


#include <stddef.h>
#include <stdint.h>


#include "abi_common.h"   // Shared/include
#include "stl_core.h"     // Lib-CPP-static/include
#include "lib-cpp-api.h"  // Lib-CPP-static/include


#ifdef _WIN32
#ifdef BUILDING_DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else
#define DLL_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // 기본 기능
    DLL_API int imp_add(int a, int b);
    DLL_API int imp_sub(int a, int b);

    // Widget 테스트(내부에서 STL/Widget 사용)
    DLL_API int imp_widget_sum_range(int start, int count);

    // ABI
    DLL_API void    imp_abi_get_info(AbiInfo* outInfo);
    DLL_API int32_t imp_abi_check_compat(const AbiExpected* exp);

    // STL-safe(핸들) — stl_core 래핑(심볼 충돌 방지 위해 imp_ prefix)
    typedef struct StlHandle StlHandle;

    DLL_API StlHandle* imp_stl_create(uint32_t seed);
    DLL_API void       imp_stl_destroy(StlHandle* h);
    DLL_API int        imp_stl_set_name(StlHandle* h, const char* nameUtf8);
    DLL_API int        imp_stl_get_name(StlHandle* h, char* outBuf, size_t outBufBytes, size_t* outRequiredBytes);
    DLL_API char* imp_stl_alloc_name(StlHandle* h);
    DLL_API void       imp_stl_free(char* p);
    DLL_API int        imp_stl_push(StlHandle* h, int v);
    DLL_API size_t     imp_stl_count(StlHandle* h);
    DLL_API int        imp_stl_get_values(StlHandle* h, int* outArr, size_t outCount, size_t* outRequiredCount);
    DLL_API int        imp_stl_sum(StlHandle* h);

#ifdef __cplusplus
}
#endif
