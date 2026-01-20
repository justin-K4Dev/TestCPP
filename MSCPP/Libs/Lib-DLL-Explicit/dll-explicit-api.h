#pragma once


#include <stdint.h>
#include <stddef.h>


#include "abi_common.h"
#include "stl_core.h"     // StlHandle typedef
#include "lib-cpp-api.h"


#ifdef _WIN32
#define PLUGIN_CALL __cdecl
#else
#define PLUGIN_CALL
#endif


typedef struct PluginApi
{
    // --- ABI ---
    AbiInfo abi;

    // --- basic ---
    int (PLUGIN_CALL* add)(int, int);
    int (PLUGIN_CALL* sub)(int, int);
    int (PLUGIN_CALL* widget_sum_range)(int start, int count);

    // --- STL-safe (no export name conflict: function pointers only) ---
    StlHandle* (PLUGIN_CALL* stl_create)(uint32_t seed);
    void       (PLUGIN_CALL* stl_destroy)(StlHandle* h);
    int        (PLUGIN_CALL* stl_set_name)(StlHandle* h, const char* nameUtf8);
    int        (PLUGIN_CALL* stl_get_name)(StlHandle* h, char* outBuf, size_t outBufBytes, size_t* outRequiredBytes);
    char* (PLUGIN_CALL* stl_alloc_name)(StlHandle* h);
    void       (PLUGIN_CALL* stl_free)(char* p);
    int        (PLUGIN_CALL* stl_push)(StlHandle* h, int v);
    size_t(PLUGIN_CALL* stl_count)(StlHandle* h);
    int        (PLUGIN_CALL* stl_get_values)(StlHandle* h, int* outArr, size_t outCount, size_t* outRequiredCount);
    int        (PLUGIN_CALL* stl_sum)(StlHandle* h);

    // ABI check helper
    int32_t(PLUGIN_CALL* abi_check_compat)(const AbiExpected* exp);

} PluginApi;

#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) int PLUGIN_CALL plugin_get_api(PluginApi* outApi);
    typedef int (PLUGIN_CALL* plugin_get_api_fn)(PluginApi* outApi);

#ifdef __cplusplus
}
#endif
