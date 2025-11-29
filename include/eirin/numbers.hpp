#ifndef EIRIN_FIXED_NUMBERS_HPP
#define EIRIN_FIXED_NUMBERS_HPP

#pragma once

#include <eirin/fixed.hpp>

namespace eirin::numbers
{
template <typename T>
using enable_if_fixed = std::enable_if_t<is_fixed_point_v<T>, T>;

// e
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> e_v()
{
    return T::template from_fixed_num_value<61>(0x56FC2A2C515DA54Dll);
}

// pi
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> pi_v()
{
    return T::template from_fixed_num_value<61>(0x6487ED5110B4611All);
}

// log2e
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> log2e_v()
{
    return T::template from_fixed_num_value<61>(0x2e2a8eca5705fc00ll);
}

// log10e
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> log10e_v()
{
    return T::template from_fixed_num_value<61>(0xde5bd8a93728700ll);
}

// inv_pi
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> inv_pi_v()
{
    return T::template from_fixed_num_value<61>(0xa2f9836e4e44180ll);
}

// inv_sqrtpi
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> inv_sqrtpi_v()
{
    return T::template from_fixed_num_value<61>(0x120dd750429b6d00ll);
}

// ln2
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> ln2_v()
{
    return T::template from_fixed_num_value<61>(0x162e42fefa39ef00ll);
}

// ln10
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> ln10_v()
{
    return T::template from_fixed_num_value<61>(0x49aec6eed5545800ll);
}

// sqrt2
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> sqrt2_v()
{
    return T::template from_fixed_num_value<61>(0x2d413cccfe779a00ll);
}

// sqrt3
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> sqrt3_v()
{
    return T::template from_fixed_num_value<61>(0x376cf5d0b0995400ll);
}

// inv_sqrt3
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> inv_sqrt3_v()
{
    return T::template from_fixed_num_value<61>(0x1279a74590331c00ll);
}

// egamma
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> egamma_v()
{
    return T::template from_fixed_num_value<61>(0x12788cfc6fb61900ll);
}

// phi
template <typename T>
EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> phi_v()
{
    return T::template from_fixed_num_value<61>(0x33c6ef372fe95000ll);
}

inline constexpr auto pi = pi_v<fixed32>();
inline constexpr auto e = e_v<fixed32>();
inline constexpr auto log2e = log2e_v<fixed32>();
inline constexpr auto log10e = log10e_v<fixed32>();
inline constexpr auto inv_pi = inv_pi_v<fixed32>();
inline constexpr auto inv_sqrtpi = inv_sqrtpi_v<fixed32>();
inline constexpr auto ln2 = ln2_v<fixed32>();
inline constexpr auto ln10 = ln10_v<fixed32>();
inline constexpr auto sqrt2 = sqrt2_v<fixed32>();
inline constexpr auto sqrt3 = sqrt3_v<fixed32>();
inline constexpr auto inv_sqrt3 = inv_sqrt3_v<fixed32>();
inline constexpr auto egamma = egamma_v<fixed32>();
inline constexpr auto phi = phi_v<fixed32>();

#ifdef EIRIN_FIXED_HAS_INT128
inline constexpr auto pi_f64 = pi_v<fixed64>();
inline constexpr auto e_f64 = e_v<fixed64>();
inline constexpr auto log2e_f64 = log2e_v<fixed64>();
inline constexpr auto log10e_f64 = log10e_v<fixed64>();
inline constexpr auto inv_pi_f64 = inv_pi_v<fixed64>();
inline constexpr auto inv_sqrtpi_f64 = inv_sqrtpi_v<fixed64>();
inline constexpr auto ln2_f64 = ln2_v<fixed64>();
inline constexpr auto ln10_f64 = ln10_v<fixed64>();
inline constexpr auto sqrt2_f64 = sqrt2_v<fixed64>();
inline constexpr auto sqrt3_f64 = sqrt3_v<fixed64>();
inline constexpr auto inv_sqrt3_f64 = inv_sqrt3_v<fixed64>();
inline constexpr auto egamma_f64 = egamma_v<fixed64>();
inline constexpr auto phi_f64 = phi_v<fixed64>();
#endif

} // namespace eirin::numbers

#endif
