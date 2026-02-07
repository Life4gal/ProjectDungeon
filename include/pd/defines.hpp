// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <pd/version.hpp>

// __cpp_lib_xxx
#include <version>
// std::unreachable()
#include <utility>

#if __has_include(<ciso646>)
#include <ciso646>
#endif

// =========================================================
// COMPILER
// =========================================================

#if defined(PD_COMPILER_MSVC)

#if __cpp_lib_unreachable >= 202202L
#define PD_COMPILER_UNREACHABLE() std::unreachable()
#else
#define PD_COMPILER_UNREACHABLE() __assume(0)
#endif

#define PD_COMPILER_DEBUG_TRAP() __debugbreak()
#define PD_COMPILER_IMPORTED_SYMBOL __declspec(dllimport)
#define PD_COMPILER_EXPORTED_SYMBOL __declspec(dllexport)
#define PD_COMPILER_LOCAL_SYMBOL

#define PD_COMPILER_DISABLE_WARNING_PUSH __pragma(warning(push))
#define PD_COMPILER_DISABLE_WARNING_POP __pragma(warning(pop))
#define PD_COMPILER_DISABLE_WARNING(warningNumber) __pragma(warning(disable : warningNumber)) // NOLINT(bugprone-macro-parentheses)

#elif defined(PD_COMPILER_GNU)

#if __cpp_lib_unreachable >= 202202L
#define PD_COMPILER_UNREACHABLE() std::unreachable()
#else
#define PD_COMPILER_UNREACHABLE() __builtin_unreachable()
#endif

#define PD_COMPILER_DEBUG_TRAP() __builtin_trap()
#define PD_COMPILER_IMPORTED_SYMBOL __attribute__((visibility("default")))
#define PD_COMPILER_EXPORTED_SYMBOL __attribute__((visibility("default")))
#define PD_COMPILER_LOCAL_SYMBOL __attribute__((visibility("hidden")))

#define PD_COMPILER_DISABLE_WARNING_PUSH _Pragma("GCC diagnostic push")
#define PD_COMPILER_DISABLE_WARNING_POP _Pragma("GCC diagnostic pop")

#define PD_COMPILER_PRIVATE_DO_PRAGMA(X) _Pragma(#X)
#define PD_COMPILER_DISABLE_WARNING(warningName) PD_COMPILER_PRIVATE_DO_PRAGMA(GCC diagnostic ignored #warningName)

#elif defined(PD_COMPILER_APPLE_CLANG) or defined(PD_COMPILER_CLANG_CL) or defined(PD_COMPILER_CLANG)

#if __cpp_lib_unreachable >= 202202L
#define PD_COMPILER_UNREACHABLE() std::unreachable()
#else
#define PD_COMPILER_UNREACHABLE() __builtin_unreachable()
#endif

#define PD_COMPILER_DEBUG_TRAP() __builtin_trap()
#define PD_COMPILER_IMPORTED_SYMBOL __attribute__((visibility("default")))
#define PD_COMPILER_EXPORTED_SYMBOL __attribute__((visibility("default")))
#define PD_COMPILER_LOCAL_SYMBOL __attribute__((visibility("hidden")))

#define PD_COMPILER_DISABLE_WARNING_PUSH _Pragma("clang diagnostic push")
#define PD_COMPILER_DISABLE_WARNING_POP _Pragma("clang diagnostic pop")

#define PD_COMPILER_PRIVATE_DO_PRAGMA(X) _Pragma(#X)
#define PD_COMPILER_DISABLE_WARNING(warningName) PD_COMPILER_PRIVATE_DO_PRAGMA(clang diagnostic ignored #warningName)

#else

#define PD_COMPILER_UNREACHABLE() std::unreachable()
#define PD_COMPILER_DEBUG_TRAP()
#define PD_COMPILER_IMPORTED_SYMBOL
#define PD_COMPILER_EXPORTED_SYMBOL
#define PD_COMPILER_LOCAL_SYMBOL

#define PD_COMPILER_DISABLE_WARNING_PUSH
#define PD_COMPILER_DISABLE_WARNING_POP
#define PD_COMPILER_DISABLE_WARNING(warningName)

#endif

#if defined(PD_COMPILER_MSVC)
#define PD_COMPILER_DISABLE_MSVC_WARNING(warningNumber) PD_COMPILER_DISABLE_WARNING(warningNumber)
#else
#define PD_COMPILER_DISABLE_MSVC_WARNING(warningNumber)
#endif

#if defined(PD_COMPILER_GNU)
#define PD_COMPILER_DISABLE_GNU_WARNING(warningName) PD_COMPILER_DISABLE_WARNING(warningName)
#else
#define PD_COMPILER_DISABLE_GNU_WARNING(warningName)
#endif

#if defined(PD_COMPILER_APPLE_CLANG) or defined(PD_COMPILER_CLANG_CL) or defined(PD_COMPILER_CLANG)
#define PD_COMPILER_DISABLE_CLANG_WARNING(warningName) PD_COMPILER_DISABLE_WARNING(warningName)
#else
#define PD_COMPILER_DISABLE_CLANG_WARNING(warningName)
#endif

#if defined(PD_COMPILER_MSVC)
#define PD_COMPILER_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif defined(PD_COMPILER_CLANG_CL)
#if __clang_major__ >= 18
// https://github.com/llvm/llvm-project/pull/65675
// https://github.com/llvm/llvm-project/pull/67199
#define PD_COMPILER_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define PD_COMPILER_NO_UNIQUE_ADDRESS
#endif
#else
#define PD_COMPILER_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

#if defined(PD_COMPILER_APPLE_CLANG) or defined(PD_COMPILER_CLANG_CL) or defined(PD_COMPILER_CLANG)
#define PD_COMPILER_NO_DESTROY [[clang::no_destroy]]
#else
#define PD_COMPILER_NO_DESTROY
#endif

#if defined(PD_COMPILER_MSVC)
#define PD_COMPILER_EMPTY_BASE __declspec(empty_bases)
#else
#define PD_COMPILER_EMPTY_BASE
#endif

#if defined(PD_COMPILER_MSVC)
#define PD_COMPILER_LINK2005_INLINE inline
#else
#define PD_COMPILER_LINK2005_INLINE
#endif

#if defined(NDEBUG) or defined(_NDEBUG)
#define PD_COMPILER_DEBUG 0
#else
#define PD_COMPILER_DEBUG 1
#endif

// =========================================================
// DEBUGGING
// =========================================================

#ifndef PD_ASSERT
#include <cassert>
#define PD_ASSERT assert
#endif
