#pragma once

#include <stdint.h>

#ifdef _WIN32
  #define ABI_CALL __cdecl
#else
  #define ABI_CALL
#endif

// ABI 메이저/마이너: 구조가 깨지면 major 올리고, 확장은 minor로
#define ABI_MAJOR 1
#define ABI_MINOR 0

// CRT/빌드 플래그를 비트로 기록 (런타임에서 서로 비교 가능)
enum AbiCrtFlags : uint32_t
{
    ABI_CRT_MT    = 1u << 0, // _MT
    ABI_CRT_DLL   = 1u << 1, // _DLL (/MD /MDd)
    ABI_CRT_DEBUG = 1u << 2, // _DEBUG
};

static inline uint32_t abi_crt_flags()
{
    uint32_t f = 0;
#ifdef _MT
    f |= ABI_CRT_MT;
#endif
#ifdef _DLL
    f |= ABI_CRT_DLL;
#endif
#ifdef _DEBUG
    f |= ABI_CRT_DEBUG;
#endif
    return f;
}

static inline uint32_t abi_iterator_debug_level()
{
#ifdef _ITERATOR_DEBUG_LEVEL
    return (uint32_t)_ITERATOR_DEBUG_LEVEL;
#else
    return 0;
#endif
}

// 라이브러리가 제공하는 ABI 정보
typedef struct AbiInfo
{
    uint32_t abi_major;
    uint32_t abi_minor;
    uint32_t struct_size;           // sizeof(AbiInfo)
    uint32_t crt_flags;             // AbiCrtFlags bitmask
    uint32_t iterator_debug_level;  // MSVC STL 디버그 레벨

    // toolset/플랫폼 구분용 (최소 기본)
    uint32_t msvc_ver;              // _MSC_VER (MSVC면), 아니면 0
    uint32_t is_64bit;              // 1이면 64bit
    uint32_t reserved0;

} AbiInfo;

// 호출자가 기대하는 ABI
typedef struct AbiExpected
{
    uint32_t expect_abi_major;         // 보통 ABI_MAJOR
    uint32_t min_abi_minor;            // 최소 minor
    uint32_t expect_iterator_debug;    // 0/2 등
    uint32_t require_same_crt_flags;   // 1이면 crt_flags 완전 일치 요구
    uint32_t expected_crt_flags;       // require_same_crt_flags=1일 때 사용
    uint32_t reserved[3];
} AbiExpected;

// 라이브러리 내부에서 검증 결과를 리턴(0=성공)
enum AbiCheckResult : int32_t
{
    ABI_OK = 0,
    ABI_ERR_NULL = -1,
    ABI_ERR_MAJOR_MISMATCH = -2,
    ABI_ERR_MINOR_TOO_OLD = -3,
    ABI_ERR_ITERATOR_DEBUG_MISMATCH = -4,
    ABI_ERR_CRT_FLAGS_MISMATCH = -5,
    ABI_ERR_ARCH_MISMATCH = -6,
};

static inline void fill_abi_info(AbiInfo* out)
{
    if (!out) return;
    out->abi_major = ABI_MAJOR;
    out->abi_minor = ABI_MINOR;
    out->struct_size = (uint32_t)sizeof(AbiInfo);
    out->crt_flags = abi_crt_flags();
    out->iterator_debug_level = abi_iterator_debug_level();

#ifdef _MSC_VER
    out->msvc_ver = (uint32_t)_MSC_VER;
#else
    out->msvc_ver = 0;
#endif
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
    out->is_64bit = 1;
#else
    out->is_64bit = 0;
#endif

    out->reserved0 = 0;
}

static inline int32_t check_abi_compat(const AbiExpected* exp, const AbiInfo* me)
{
    if (!exp || !me) return ABI_ERR_NULL;
    if (me->abi_major != exp->expect_abi_major) return ABI_ERR_MAJOR_MISMATCH;
    if (me->abi_minor < exp->min_abi_minor)     return ABI_ERR_MINOR_TOO_OLD;
    if (me->iterator_debug_level != exp->expect_iterator_debug)
        return ABI_ERR_ITERATOR_DEBUG_MISMATCH;

    if (exp->require_same_crt_flags && me->crt_flags != exp->expected_crt_flags)
        return ABI_ERR_CRT_FLAGS_MISMATCH;

#if defined(_WIN64)
    if (me->is_64bit != 1) return ABI_ERR_ARCH_MISMATCH;
#else
    if (me->is_64bit != 0) return ABI_ERR_ARCH_MISMATCH;
#endif

    return ABI_OK;
}




