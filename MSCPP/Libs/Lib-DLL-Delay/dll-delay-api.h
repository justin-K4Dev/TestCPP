#pragma once


#include <stddef.h>
#include <stdint.h>


#include "abi_common.h"
#include "stl_core.h"
#include "lib-cpp-api.h"



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

    DLL_API int delay_add(int a, int b);
    DLL_API int delay_sub(int a, int b);

    DLL_API int delay_widget_sum_range(int start, int count);

    DLL_API void    delay_abi_get_info(AbiInfo* outInfo);
    DLL_API int32_t delay_abi_check_compat(const AbiExpected* exp);

    // STL-safe wrappers (delay_ prefix)
    typedef struct StlHandle StlHandle;

    DLL_API StlHandle* delay_stl_create(uint32_t seed);
    DLL_API void       delay_stl_destroy(StlHandle* h);
    DLL_API int        delay_stl_set_name(StlHandle* h, const char* nameUtf8);
    DLL_API int        delay_stl_get_name(StlHandle* h, char* outBuf, size_t outBufBytes, size_t* outRequiredBytes);
    DLL_API char* delay_stl_alloc_name(StlHandle* h);
    DLL_API void       delay_stl_free(char* p);
    DLL_API int        delay_stl_push(StlHandle* h, int v);
    DLL_API size_t     delay_stl_count(StlHandle* h);
    DLL_API int        delay_stl_get_values(StlHandle* h, int* outArr, size_t outCount, size_t* outRequiredCount);
    DLL_API int        delay_stl_sum(StlHandle* h);

#ifdef __cplusplus
}
#endif