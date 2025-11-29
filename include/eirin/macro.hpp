#ifndef EIRIN_FIXED_MARCO_HPP
#define EIRIN_FIXED_MARCO_HPP

#pragma once

#ifdef __GNUC__
#    ifndef __clang__
#        define EIRIN_ALWAYS_INLINE __attribute__((always_inline)) inline
#    else
#        define EIRIN_ALWAYS_INLINE [[gnu::always_inline]] inline
#    endif
#elif defined _MSC_VER
#    define EIRIN_ALWAYS_INLINE __forceinline
#else
#    define EIRIN_ALWAYS_INLINE inline
#endif

// operate system detection
#if defined(_WIN32) || defined(_WIN64)
#    define EIRIN_OS_WINDOWS
#elif defined(__linux__) || defined(__linux)
#    define EIRIN_OS_LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#    define EIRIN_OS_MACOS
#else
#    define EIRIN_OS_UNKNOWN
#endif

// enable benchmark file input mode when on linux/windows/macos
#if defined(EIRIN_OS_LINUX) || defined(EIRIN_OS_WINDOWS) || defined(EIRIN_OS_MACOS)
#    define EIRIN_BENCHMARK_FILE_INPUT_MODE
#else
#    define EIRIN_BENCHMARK_COMMON_TEST_MODE
#endif

#endif // EIRIN_FIXED_MARCO_HPP
