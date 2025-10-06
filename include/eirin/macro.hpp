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

#ifndef __EXCEPTIONS
#    define EIRIN_NO_EXCEPTIONS
#endif

#endif // EIRIN_FIXED_MARCO_HPP
