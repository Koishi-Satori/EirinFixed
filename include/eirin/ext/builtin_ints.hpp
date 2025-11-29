#ifndef EIRIN_FIXED_EXT_BUILTIN_INTS_HPP
#define EIRIN_FIXED_EXT_BUILTIN_INTS_HPP

#pragma once

#include <cstdint>
#include <concepts>
#include <limits>
#include <eirin/macro.hpp>

#define EIRIN_FIXED_HAS_INT128_EXT

namespace eirin::ext
{
class int128
{
public:
    inline int128() noexcept = default;
    int128(const int128&) noexcept = default;

    template <std::integral T>
    requires(sizeof(T) <= 8)
    EIRIN_ALWAYS_INLINE explicit int128(T val) noexcept
        : high(0), low(val){};

    template <std::floating_point T>
    requires(sizeof(T) <= 8)
    EIRIN_ALWAYS_INLINE explicit int128(T val) noexcept
        : high(0), low(static_cast<int64_t>(val)){};

    template <std::integral T>
    requires(sizeof(T) <= 8)
    EIRIN_ALWAYS_INLINE constexpr explicit int128(T high, T low) noexcept
        : high(static_cast<int64_t>(high)), low(static_cast<int64_t>(low)){};

    EIRIN_ALWAYS_INLINE constexpr int64_t high_bits() noexcept
    {
        return high;
    }

    EIRIN_ALWAYS_INLINE constexpr int64_t low_bits() noexcept
    {
        return low;
    }

    EIRIN_ALWAYS_INLINE constexpr explicit operator int64_t() const noexcept
    {
        // when casting, we should consider about overflow.
        if(high > 0)
            return std::numeric_limits<int64_t>::max();
        else if(high < 0)
            return std::numeric_limits<int64_t>::min();
        else
            return low;
    }

    EIRIN_ALWAYS_INLINE constexpr int128 operator+(const int128& rhs) const noexcept
    {
        auto h = high + rhs.high;
        auto l = low + rhs.low;
        return int128(h, l);
    }

private:
    int64_t high;
    int64_t low;
};
} // namespace eirin::ext

#endif
