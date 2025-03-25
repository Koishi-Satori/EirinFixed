#ifndef EIRIN_FIXED_MARCO_HPP
#define EIRIN_FIXED_MARCO_HPP

#pragma once

#ifdef __GNUC__
#    ifndef __clang__
#        define EIRIN_ALWAYS_INLINE __attribute__((always_inline))
#    else
#        define EIRIN_ALWAYS_INLINE [[gnu::always_inline]]
#    endif
#elif defined _MSC_VER
#    define EIRIN_ALWAYS_INLINE __forceinline
#else
#    define EIRIN_ALWAYS_INLINE
#endif

// Define if should provide std::is_signed<_Tp> specialization for int128_type.
// undef it before include this file if you need to avoid redefinition.
#define EIRIN_DEFINE_INT128_SIGNED 0

#endif // EIRIN_FIXED_MARCO_HPP
