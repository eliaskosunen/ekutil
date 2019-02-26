// Copyright 2018-2019 Elias Kosunen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file is a part of ekutil:
//     https://github.com/eliaskosunen/ekutil

#ifndef EKUTIL_BITS_COMPAT_H
#define EKUTIL_BITS_COMPAT_H

#define EKUTIL_STD_11 201103L
#define EKUTIL_STD_14 201402L
#define EKUTIL_STD_17 201703L

#define EKUTIL_COMPILER(major, minor, patch) \
    ((major)*10000000 /* 10,000,000 */ + (minor)*10000 /* 10,000 */ + (patch))
#define EKUTIL_VERSION EKUTIL_COMPILER(0, 0, 1)

#ifdef __INTEL_COMPILER
// Intel
#define EKUTIL_INTEL                                                      \
    EKUTIL_COMPILER(__INTEL_COMPILER / 100, (__INTEL_COMPILER / 10) % 10, \
                    __INTEL_COMPILER % 10)
#elif defined(_MSC_VER) && defined(_MSC_FULL_VER)
// MSVC
#if _MSC_VER == _MSC_FULL_VER / 10000
#define EKUTIL_MSVC \
    EKUTIL_COMPILER(_MSC_VER / 100, _MSC_VER % 100, _MSC_FULL_VER % 10000)
#else
#define EKUTIL_MSVC                                                \
    EKUTIL_COMPILER(_MSC_VER / 100, (_MSC_FULL_VER / 100000) % 10, \
                    _MSC_FULL_VER % 100000)
#endif  // _MSC_VER == _MSC_FULL_VER / 10000
#elif defined(__clang__) && defined(__clang_minor__) && \
    defined(__clang_patchlevel__)
// Clang
#define EKUTIL_CLANG \
    EKUTIL_COMPILER(__clang_major__, __clang_minor__, __clang_patchlevel__)
#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    defined(__GNUC_PATCHLEVEL__)
// GCC
#define EKUTIL_GCC \
    EKUTIL_COMPILER(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#endif

#ifndef EKUTIL_INTEL
#define EKUTIL_INTEL 0
#endif
#ifndef EKUTIL_MSVC
#define EKUTIL_MSVC 0
#endif
#ifndef EKUTIL_CLANG
#define EKUTIL_CLANG 0
#endif
#ifndef EKUTIL_GCC
#define EKUTIL_GCC 0
#endif

#define EKUTIL_STRINGIFY_APPLY(x) #x
#define EKUTIL_STRINGIFY(x) EKUTIL_STRINGIFY_APPLY(x)

// Warning control
#if EKUTIL_GCC
#define EKUTIL_PRAGMA_APPLY(x) _Pragma(#x)

#define EKUTIL_GCC_PUSH _Pragma("GCC diagnostic push")
#define EKUTIL_GCC_POP _Pragma("GCC diagnostic pop")

#define EKUTIL_GCC_IGNORE(x) EKUTIL_PRAGMA_APPLY(GCC diagnostic ignored x)
#else
#define EKUTIL_GCC_PUSH
#define EKUTIL_GCC_POP
#define EKUTIL_GCC_IGNORE(x)
#endif

#if EKUTIL_CLANG
#define EKUTIL_PRAGMA_APPLY(x) _Pragma(#x)

#define EKUTIL_CLANG_PUSH _Pragma("clang diagnostic push")
#define EKUTIL_CLANG_POP _Pragma("clang diagnostic pop")

#define EKUTIL_CLANG_IGNORE(x) EKUTIL_PRAGMA_APPLY(clang diagnostic ignored x)

#if EKUTIL_CLANG >= EKUTIL_COMPILER(3, 9, 0)
#define EKUTIL_CLANG_PUSH_IGNORE_UNDEFINED_TEMPLATE \
    EKUTIL_CLANG_PUSH EKUTIL_CLANG_IGNORE("-Wundefined-func-template")
#define EKUTIL_CLANG_POP_IGNORE_UNDEFINED_TEMPLATE EKUTIL_CLANG_POP
#else
#define EKUTIL_CLANG_PUSH_IGNORE_UNDEFINED_TEMPLATE
#define EKUTIL_CLANG_POP_IGNORE_UNDEFINED_TEMPLATE
#endif

#else
#define EKUTIL_CLANG_PUSH
#define EKUTIL_CLANG_POP
#define EKUTIL_CLANG_IGNORE(x)
#define EKUTIL_CLANG_PUSH_IGNORE_UNDEFINED_TEMPLATE
#define EKUTIL_CLANG_POP_IGNORE_UNDEFINED_TEMPLATE
#endif

#if EKUTIL_MSVC
#define EKUTIL_MSVC_PUSH __pragma(warning(push))
#define EKUTIL_MSVC_POP __pragma(warning(pop))

#define EKUTIL_MSVC_IGNORE(x) __pragma(warning(disable : x))
#else
#define EKUTIL_MSVC_PUSH
#define EKUTIL_MSVC_POP
#define EKUTIL_MSVC_IGNORE(x)
#endif

#ifdef __has_include
#define EKUTIL_HAS_INCLUDE(x) __has_include(x)
#else
#define EKUTIL_HAS_INCLUDE(x) 0
#endif

#ifdef __has_cpp_attribute
#define EKUTIL_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define EKUTIL_HAS_CPP_ATTRIBUTE(x) 0
#endif

#ifdef __has_feature
#define EKUTIL_HAS_FEATURE(x) __has_feature(x)
#else
#define EKUTIL_HAS_FEATURE(x) 0
#endif

#ifdef __has_builtin
#define EKUTIL_HAS_BUILTIN(x) __has_builtin(x)
#else
#define EKUTIL_HAS_BUILTIN(x) 0
#endif

// Detect constexpr
#if defined(__cpp_constexpr)
#if __cpp_constexpr >= 201304
#define EKUTIL_HAS_RELAXED_CONSTEXPR 1
#else
#define EKUTIL_HAS_RELAXED_CONSTEXPR 0
#endif
#endif

#ifndef EKUTIL_HAS_RELAXED_CONSTEXPR
#if EKUTIL_HAS_FEATURE(cxx_relaxed_constexpr) ||    \
    EKUTIL_MSVC >= EKUTIL_COMPILER(19, 10, 0) ||    \
    ((EKUTIL_GCC >= EKUTIL_COMPILER(6, 0, 0) ||     \
      EKUTIL_INTEL >= EKUTIL_COMPILER(17, 0, 0)) && \
     __cplusplus >= EKUTIL_STD_14)
#define EKUTIL_HAS_RELAXED_CONSTEXPR 1
#else
#define EKUTIL_HAS_RELAXED_CONSTEXPR 0
#endif
#endif

#if EKUTIL_HAS_RELAXED_CONSTEXPR
#define EKUTIL_CONSTEXPR constexpr
#define EKUTIL_CONSTEXPR14 constexpr
#define EKUTIL_CONSTEXPR_DECL constexpr
#else
#define EKUTIL_CONSTEXPR constexpr
#define EKUTIL_CONSTEXPR14 inline
#define EKUTIL_CONSTEXPR_DECL
#endif

// Detect [[nodiscard]]
#if (EKUTIL_HAS_CPP_ATTRIBUTE(nodiscard) && __cplusplus >= EKUTIL_STD_17) || \
    (EKUTIL_MSVC >= EKUTIL_COMPILER(19, 11, 0) &&                            \
     EKUTIL_MSVC_LANG >= EKUTIL_STD_17) ||                                   \
    ((EKUTIL_GCC >= EKUTIL_COMPILER(7, 0, 0) ||                              \
      EKUTIL_INTEL >= EKUTIL_COMPILER(18, 0, 0)) &&                          \
     __cplusplus >= EKUTIL_STD_17)
#define EKUTIL_NODISCARD [[nodiscard]]
#else
#define EKUTIL_NODISCARD /*nodiscard*/
#endif

#if defined(EKUTIL_HEADER_ONLY) && EKUTIL_HEADER_ONLY
#define EKUTIL_FUNC inline
#else
#define EKUTIL_FUNC
#endif

// Detect <charconv>
#if (defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201606) || \
    EKUTIL_HAS_INCLUDE(<charconv>)

#if defined(_GLIBCXX_RELEASE) && _GLIBCXX_RELEASE >= EKUTIL_COMPILER(8, 1, 0)
#define EKUTIL_HAS_INTEGER_CHARCONV 1
#define EKUTIL_HAS_FLOAT_CHARCONV 0
#elif EKUTIL_MSVC >= EKUTIL_COMPILER(19, 14, 0)
#define EKUTIL_HAS_INTEGER_CHARCONV 1
#define EKUTIL_HAS_FLOAT_CHARCONV 0
#endif

#endif

#ifndef EKUTIL_HAS_INTEGER_CHARCONV
#define EKUTIL_HAS_INTEGER_CHARCONV 0
#define EKUTIL_HAS_FLOAT_CHARCONV 0
#endif

// Detect std::launder
#if defined(__cpp_lib_launder) && __cpp_lib_launder >= 201606
#define EKUTIL_HAS_LAUNDER 1
#else
#define EKUTIL_HAS_LAUNDER 0
#endif

// Detect void_t
#if defined(__cpp_lib_void_t) && __cpp_lib_void_t >= 201411
#define EKUTIL_HAS_VOID_T 1
#else
#define EKUTIL_HAS_VOID_T 0
#endif

// Detect __assume
#if EKUTIL_INTEL || EKUTIL_MSVC
#define EKUTIL_HAS_ASSUME 1
#else
#define EKUTIL_HAS_ASSUME 0
#endif

// Detect __builtin_assume
#if EKUTIL_HAS_BUILTIN(__builtin_assume)
#define EKUTIL_HAS_BUILTIN_ASSUME 1
#else
#define EKUTIL_HAS_BUILTIN_ASSUME 0
#endif

// Detect __builtin_unreachable
#if EKUTIL_HAS_BUILTIN(__builtin_unreachable) || EKUTIL_GCC
#define EKUTIL_HAS_BUILTIN_UNREACHABLE 1
#else
#define EKUTIL_HAS_BUILTIN_UNREACHABLE 0
#endif

#if EKUTIL_HAS_ASSUME
#define EKUTIL_ASSUME(x) __assume(x)
#elif EKUTIL_HAS_BUILTIN_ASSUME
#define EKUTIL_ASSUME(x) __builtin_assume(x)
#elif EKUTIL_HAS_BUILTIN_UNREACHABLE
#define EKUTIL_ASSUME(x)             \
    do {                             \
        if (!(x)) {                  \
            __builtin_unreachable(); \
        }                            \
    } while (false)
#else
#define EKUTIL_ASSUME(x) static_cast<void>(sizeof(x))
#endif

#if EKUTIL_HAS_BUILTIN_UNREACHABLE
#define EKUTIL_UNREACHABLE __builtin_unreachable()
#else
#define EKUTIL_UNREACHABLE EKUTIL_ASSUME(0)
#endif

// Detect __builtin_expect
#if EKUTIL_HAS_BUILTIN(__builtin_expect) || EKUTIL_GCC || EKUTIL_CLANG
#define EKUTIL_HAS_BUILTIN_EXPECT 1
#else
#define EKUTIL_HAS_BUILTIN_EXPECT 0
#endif

#if EKUTIL_HAS_BUILTIN_EXPECT
#define EKUTIL_LIKELY(x) __builtin_expect(!!(x), 1)
#define EKUTIL_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define EKUTIL_LIKELY(x) (x)
#define EKUTIL_UNLIKELY(x) (x)
#endif

#define EKUTIL_UNUSED(x) static_cast<void>(sizeof(x))

#ifndef EKUTIL_STL_OVERLOADS
#define EKUTIL_STL_OVERLOADS 1
#endif

#endif  // EKUTIL_BITS_COMPAT_H
