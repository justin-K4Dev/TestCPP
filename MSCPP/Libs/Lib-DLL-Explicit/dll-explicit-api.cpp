#include "stdafx.h"
#include "dll-explicit-api.h"


// 내부 구현(테이블에만 넣음)
static int PLUGIN_CALL add_impl(int a, int b) { return a + b + 2000; }
static int PLUGIN_CALL sub_impl(int a, int b) { return a - b - 2000; }

static int PLUGIN_CALL widget_sum_range_impl(int start, int count)
{
    auto v = LibCpp::make_range(start, count);
    LibCpp::Widget w("plugin");
    for (int x : v) w.add_value(x);
    return w.sum() + 2000;
}

static int32_t PLUGIN_CALL abi_check_impl(const AbiExpected* exp)
{
    AbiInfo me{};
    fill_abi_info(&me);
    return check_abi_compat(exp, &me);
}

extern "C" __declspec(dllexport)
int PLUGIN_CALL plugin_get_api(PluginApi* outApi)
{
    if (!outApi) return 0;
    std::memset(outApi, 0, sizeof(*outApi));

    fill_abi_info(&outApi->abi);

    outApi->add = &add_impl;
    outApi->sub = &sub_impl;
    outApi->widget_sum_range = &widget_sum_range_impl;

    // stl_core_*를 그대로 함수 포인터로 제공 (export 심볼 충돌 X)
    outApi->stl_create = &stl_core_create;
    outApi->stl_destroy = &stl_core_destroy;
    outApi->stl_set_name = &stl_core_set_name;
    outApi->stl_get_name = &stl_core_get_name;
    outApi->stl_alloc_name = &stl_core_alloc_name;
    outApi->stl_free = &stl_core_free;
    outApi->stl_push = &stl_core_push;
    outApi->stl_count = &stl_core_count;
    outApi->stl_get_values = &stl_core_get_values;
    outApi->stl_sum = &stl_core_sum;

    outApi->abi_check_compat = &abi_check_impl;

    return 1;
}
