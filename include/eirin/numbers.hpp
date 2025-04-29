#ifndef EIRIN_FIXED_NUMBERS_HPP
#define EIRIN_FIXED_NUMBERS_HPP

#include <eirin/fixed.hpp>
#include <eirin/math.hpp>
#include <cmath>

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

} // namespace eirin::numbers

#endif
