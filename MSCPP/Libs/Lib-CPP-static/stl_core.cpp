#include "stdafx.h"
#include "stl_core.h"


struct StlHandle
{
    uint32_t seed{};
    std::string name;
    std::vector<int> values;
};

extern "C" StlHandle* stl_core_create(uint32_t seed)
{
    auto* h = new (std::nothrow) StlHandle{};
    if (!h) return nullptr;
    h->seed = seed ? seed : 0x12345678u;
    h->name = "default";
    h->values.reserve(32);
    return h;
}

extern "C" void stl_core_destroy(StlHandle* h)
{
    delete h;
}

extern "C" int stl_core_set_name(StlHandle* h, const char* nameUtf8)
{
    if (!h) return -1;
    h->name = (nameUtf8 ? nameUtf8 : "");
    return 0;
}

extern "C" int stl_core_get_name(StlHandle* h, char* outBuf, size_t outBufBytes, size_t* outRequiredBytes)
{
    if (!h) return -1;

    const size_t need = h->name.size() + 1;
    if (outRequiredBytes) *outRequiredBytes = need;

    if (!outBuf || outBufBytes == 0) return 0; // size query
    if (outBufBytes < need) return 1;          // insufficient

#ifdef _MSC_VER
    memcpy_s(outBuf, outBufBytes, h->name.c_str(), need);
#else
    std::memcpy(outBuf, h->name.c_str(), need);
#endif
    return 0;
}

extern "C" char* stl_core_alloc_name(StlHandle* h)
{
    if (!h) return nullptr;

    const size_t need = h->name.size() + 1;
    char* p = (char*)std::malloc(need);
    if (!p) return nullptr;

#ifdef _MSC_VER
    memcpy_s(p, need, h->name.c_str(), need);
#else
    std::memcpy(p, h->name.c_str(), need);
#endif
    return p;
}

extern "C" void stl_core_free(char* p)
{
    std::free(p);
}

extern "C" int stl_core_push(StlHandle* h, int v)
{
    if (!h) return -1;
    h->values.push_back(v);
    return 0;
}

extern "C" size_t stl_core_count(StlHandle* h)
{
    return h ? h->values.size() : 0;
}

extern "C" int stl_core_get_values(StlHandle* h, int* outArr, size_t outCount, size_t* outRequiredCount)
{
    if (!h) return -1;

    const size_t need = h->values.size();
    if (outRequiredCount) *outRequiredCount = need;

    if (!outArr || outCount == 0) return 0; // size query
    if (outCount < need) return 1;          // insufficient

    for (size_t i = 0; i < need; ++i) outArr[i] = h->values[i];
    return 0;
}

extern "C" int stl_core_sum(StlHandle* h)
{
    if (!h) return 0;
    int s = 0;
    for (int x : h->values) s += x;
    return s;
}
