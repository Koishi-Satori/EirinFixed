#ifndef EIRIN_FIXED_DETAIL_INT128_HPP
#define EIRIN_FIXED_DETAIL_INT128_HPP

#pragma once

#include <version>

#ifdef _MSC_VER
#    define EIRIN_FIXED_HAS_INT128
// int128 provided by MSVC STL
#    define EIRIN_FIXED_DETAIL_INT128_MSVC_STL

#    include <__msvc_int128.hpp>
#endif

#ifdef __GNUC__
#    define EIRIN_FIXED_HAS_INT128
// Built-in __int128
#    define EIRIN_FIXED_DETAIL_BUILTIN__INT128
#endif

namespace eirin::detail
{
#ifdef EIRIN_FIXED_DETAIL_INT128_MSVC_STL

using int128_t = std::_Signed128;
using uint128_t = std::_Unsigned128;

#endif

#ifdef EIRIN_FIXED_DETAIL_BUILTIN__INT128

using int128_t = __int128;
using uint128_t = unsigned __int128;

#endif
} // namespace eirin::detail

#endif
