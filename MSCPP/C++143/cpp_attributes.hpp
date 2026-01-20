#pragma once
// cpp_attributes.hpp
// Cross-compiler attribute/declspec wrappers for MSVC / GCC / Clang.
// - Prefer standard [[...]] attributes when available.
// - Fall back to compiler-specific attributes/declspecs when needed.

#include <cstddef>

//
// Compiler detection
//
#if defined(_MSC_VER)
#define CPP_COMPILER_MSVC 1
#else
#define CPP_COMPILER_MSVC 0
#endif

#if defined(__clang__)
#define CPP_COMPILER_CLANG 1
#else
#define CPP_COMPILER_CLANG 0
#endif

#if defined(__GNUC__) && !CPP_COMPILER_CLANG
#define CPP_COMPILER_GCC 1
#else
#define CPP_COMPILER_GCC 0
#endif

//
// Feature detection helpers
//
#ifndef CPP_HAS_CPP_ATTRIBUTE
#if defined(__has_cpp_attribute)
#define CPP_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define CPP_HAS_CPP_ATTRIBUTE(x) 0
#endif
#endif

#ifndef CPP_HAS_ATTRIBUTE
#if defined(__has_attribute)
#define CPP_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#define CPP_HAS_ATTRIBUTE(x) 0
#endif
#endif

#ifndef CPP_HAS_BUILTIN
#if defined(__has_builtin)
#define CPP_HAS_BUILTIN(x) __has_builtin(x)
#else
#define CPP_HAS_BUILTIN(x) 0
#endif
#endif

//
// Standard attributes wrappers
//
#if CPP_HAS_CPP_ATTRIBUTE(nodiscard) >= 201603
#define CPP_NODISCARD [[nodiscard]]
#else
#define CPP_NODISCARD
#endif

#if CPP_HAS_CPP_ATTRIBUTE(maybe_unused) >= 201603
#define CPP_MAYBE_UNUSED [[maybe_unused]]
#else
#define CPP_MAYBE_UNUSED
#endif

#if CPP_HAS_CPP_ATTRIBUTE(fallthrough) >= 201603
#define CPP_FALLTHROUGH [[fallthrough]]
#else
  // GCC/Clang have fallthrough attribute too (as compiler attribute), but optional:
#if (CPP_COMPILER_GCC || CPP_COMPILER_CLANG) && CPP_HAS_ATTRIBUTE(fallthrough)
#define CPP_FALLTHROUGH __attribute__((fallthrough))
#else
#define CPP_FALLTHROUGH
#endif
#endif

#if CPP_HAS_CPP_ATTRIBUTE(noreturn) >= 200809
#define CPP_NORETURN [[noreturn]]
#else
#if CPP_COMPILER_MSVC
#define CPP_NORETURN __declspec(noreturn)
#elif (CPP_COMPILER_GCC || CPP_COMPILER_CLANG)
#define CPP_NORETURN __attribute__((noreturn))
#else
#define CPP_NORETURN
#endif
#endif

#if CPP_HAS_CPP_ATTRIBUTE(deprecated) >= 201309
#define CPP_DEPRECATED(msg) [[deprecated(msg)]]
#else
#if CPP_COMPILER_MSVC
#define CPP_DEPRECATED(msg) __declspec(deprecated(msg))
#elif (CPP_COMPILER_GCC || CPP_COMPILER_CLANG)
#define CPP_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define CPP_DEPRECATED(msg)
#endif
#endif

#if CPP_HAS_CPP_ATTRIBUTE(no_unique_address) >= 201803
#define CPP_NO_UNIQUE_ADDRESS [[no_unique_address]]
#else
#define CPP_NO_UNIQUE_ADDRESS
#endif

#if CPP_HAS_CPP_ATTRIBUTE(likely) >= 201803
#define CPP_LIKELY [[likely]]
#define CPP_UNLIKELY [[unlikely]]
#else
  // Fallback: GCC/Clang can use __builtin_expect for expressions.
#define CPP_LIKELY
#define CPP_UNLIKELY
#endif

//
// Force inline / no inline
//
#if CPP_COMPILER_MSVC
#define CPP_FORCEINLINE __forceinline
#define CPP_NOINLINE   __declspec(noinline)
#elif (CPP_COMPILER_GCC || CPP_COMPILER_CLANG)
#define CPP_FORCEINLINE inline __attribute__((always_inline))
#define CPP_NOINLINE   __attribute__((noinline))
#else
#define CPP_FORCEINLINE inline
#define CPP_NOINLINE
#endif

//
// Hot / Cold path hints
//
#if (CPP_COMPILER_GCC || CPP_COMPILER_CLANG)
#define CPP_HOT  __attribute__((hot))
#define CPP_COLD __attribute__((cold))
#else
#define CPP_HOT
#define CPP_COLD
#endif

//
// Alignment
//
#if CPP_COMPILER_MSVC
#define CPP_ALIGN(N) __declspec(align(N))
#elif (CPP_COMPILER_GCC || CPP_COMPILER_CLANG)
#define CPP_ALIGN(N) __attribute__((aligned(N)))
#else
#define CPP_ALIGN(N)
#endif

//
// Packing (struct packing) - use carefully!
//
#if CPP_COMPILER_MSVC
#define CPP_PACKED_BEGIN __pragma(pack(push, 1))
#define CPP_PACKED_END   __pragma(pack(pop))
#define CPP_PACKED
#elif (CPP_COMPILER_GCC || CPP_COMPILER_CLANG)
#define CPP_PACKED_BEGIN
#define CPP_PACKED_END
#define CPP_PACKED __attribute__((packed))
#else
#define CPP_PACKED_BEGIN
#define CPP_PACKED_END
#define CPP_PACKED
#endif

//
// Branch prediction helpers for expressions (fallback when [[likely]] not available)
//
#if (CPP_COMPILER_GCC || CPP_COMPILER_CLANG)
#define CPP_LIKELY_EXPR(x)   (__builtin_expect(!!(x), 1))
#define CPP_UNLIKELY_EXPR(x) (__builtin_expect(!!(x), 0))
#else
#define CPP_LIKELY_EXPR(x)   (x)
#define CPP_UNLIKELY_EXPR(x) (x)
#endif

//
// Assume / Unreachable
//
#if (CPP_COMPILER_GCC || CPP_COMPILER_CLANG)
#define CPP_UNREACHABLE() __builtin_unreachable()
#elif CPP_COMPILER_MSVC
#define CPP_UNREACHABLE() __assume(0)
#else
#define CPP_UNREACHABLE() ((void)0)
#endif

// C++23 [[assume(expr)]]가 있으면 그걸 쓰고,
// 없으면 "조건이 거짓이면 도달 불가"로 힌트를 줌(거짓이면 UB 가능하니 반드시 불변조건에만 사용!)
#if CPP_HAS_CPP_ATTRIBUTE(assume) >= 202207
#define CPP_ASSUME(expr) [[assume(expr)]]
#else
#define CPP_ASSUME(expr) do { if (!(expr)) CPP_UNREACHABLE(); } while (0)
#endif

//
// Example: visibility/export (선택)
// Windows DLL: __declspec(dllexport/dllimport)
// GCC/Clang: __attribute__((visibility("default")))
//
#if CPP_COMPILER_MSVC
#define CPP_DLL_EXPORT __declspec(dllexport)
#define CPP_DLL_IMPORT __declspec(dllimport)
#else
#define CPP_DLL_EXPORT __attribute__((visibility("default")))
#define CPP_DLL_IMPORT __attribute__((visibility("default")))
#endif
