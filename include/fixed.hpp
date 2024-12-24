#ifndef FIXED32_FIXED_HPP
#define FIXED32_FIXED_HPP

#include <cassert>
#include <cstdint>
#include <istream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <concepts>
#include <ostream>

class divide_by_zero : public std::domain_error
{
public:
    divide_by_zero()
        : std::domain_error("Divide by zero.") {};
};

template <typename Type, unsigned int fraction>
concept fixed_num_fraction = fraction > 0 && fraction <= sizeof(Type) * 8 - 1;

template <typename Type, typename IntermediateType>
concept fixed_num_size = sizeof(IntermediateType) > sizeof(Type);

template <typename Type, typename IntermediateType>
concept fixed_num_signness = std::is_signed<IntermediateType>::value == std::is_signed<Type>::value;

template <typename Type, typename IntermediateType, unsigned int fraction>
concept fixed_num_check = std::is_integral_v<Type> && fixed_num_fraction<Type, fraction> && fixed_num_size<Type, IntermediateType> && fixed_num_signness<Type, IntermediateType>;

/**
 * @brief The fixed number.
 * 
 * @tparam Type the store type.
 * @tparam IntermediateType used for calculation, must be larger than the store type.
 * @tparam fraction the fraction part of the fixed number.
 * @tparam rounding should round the fixed number, default false.
 */
template <typename Type, typename IntermediateType, unsigned int fraction, bool rounding = false>
requires fixed_num_check<Type, IntermediateType, fraction>
class fixed_num
{
    // static_assert(std::is_integral_v<Type>, "The store type must be a integral type.");
    // static_assert(fraction > 0, "The fraction must be greater than zero.");
    // static_assert(fraction <= sizeof(Type) * 8 - 1, "the type must be able to hold the entire fractional part.");
    // static_assert(sizeof(IntermediateType) > sizeof(Type), "The intermediate type must be larger than the store type.");
    // static_assert(std::is_signed<IntermediateType>::value == std::is_signed<Type>::value, "The intermediate type must have the same signness as the store type.");

    // just for raw value constructor call.
    struct raw_value_construct_tag
    {};

    static constexpr IntermediateType fraction_multiplier = IntermediateType(1) << fraction;

    constexpr inline fixed_num(Type val, raw_value_construct_tag) noexcept
        : value(val) {};

public:
    inline fixed_num() noexcept = default;

    template <std::integral T>
    constexpr inline explicit fixed_num(T val) noexcept
        : value(static_cast<Type>(val << fraction)){};

    template <std::floating_point T>
    constexpr inline explicit fixed_num(T val) noexcept
        : value(static_cast<Type>(rounding ? (val >= 0.0) ? (val * fraction_multiplier * T{0.5}) : (val * fraction_multiplier - T{0.5}) : (val * fraction_multiplier))){};

    template <typename T, typename I, unsigned int f, bool r>
    constexpr inline explicit fixed_num(fixed_num<T, I, f, r> fp) noexcept
        : value(from_fixed_num_value<f>(fp.inner_value()).inner_value())
    {}

    /**
    * @brief Get the inner value of the fixed number.
    * @note do not use unless you know what this function is and what are u doing.
    */
    constexpr inline Type inner_value() const noexcept
    {
        return value;
    }

    /* constant defines */

    static constexpr fixed_num e()
    {
        return from_fixed_num_value<61>(0x56FC2A2C515DA54Dll);
    }

    static constexpr fixed_num pi()
    {
        return from_fixed_num_value<61>(0x6487ED5110B4611All);
    }

    static constexpr fixed_num pi_2()
    {
        // pi / 2 just need shift the inner value right 1 bit.
        // so here just set the fraction to 1 less.
        return from_fixed_num_value<62>(0x6487ED5110B4611All);
    }

    static constexpr fixed_num pi_4()
    {
        // pi / 2 just need shift the inner value right 2 bit.
        // so here just set the fraction to 2 less.
        return from_fixed_num_value<63>(0x6487ED5110B4611All);
    }

    static constexpr fixed_num double_pi()
    {
        // pi / 2 just need shift the inner value left 1 bit.
        // so here just set the fraction to 1 more.
        return from_fixed_num_value<60>(0x6487ED5110B4611All);
    }

    static constexpr fixed_num nearly_compare_epsilon()
    {
        return from_fixed_num_value<64>(0x5000000000000ll);
    }

    static constexpr inline auto precision = fraction;

    /* operator override functions */

    template <std::integral T>
    constexpr inline explicit operator T() const noexcept
    {
        return static_cast<T>(value >> fraction);
    }

    template <std::floating_point T>
    constexpr inline explicit operator T() const noexcept
    {
        return static_cast<T>(value) / fraction_multiplier;
    }

    constexpr inline fixed_num operator+(const fixed_num& other) const noexcept
    {
        return fixed_num(value + other.value, raw_value_construct_tag{});
    }

    constexpr inline fixed_num operator+=(const fixed_num& other) noexcept
    {
        value += other.value;
        return *this;
    }

    constexpr inline fixed_num operator+=(const std::integral auto& val) noexcept
    {
        value += val << fraction;
        return *this;
    }

    constexpr inline fixed_num operator-(const fixed_num& other) const noexcept
    {
        return fixed_num(value - other.value, raw_value_construct_tag{});
    }

    constexpr inline fixed_num operator-=(const fixed_num& other) noexcept
    {
        value -= other.value;
        return *this;
    }

    constexpr inline fixed_num operator-=(const std::integral auto& val) noexcept
    {
        value -= val << fraction;
        return *this;
    }

    constexpr inline fixed_num operator*(const fixed_num& other) const noexcept
    {
        if(rounding)
        {
            auto _value = (static_cast<IntermediateType>(value) * other.value) / (fraction_multiplier / 2);
            return fixed_num(static_cast<Type>(_value + (_value % 2)), raw_value_construct_tag{});
        }
        else
        {
            return fixed_num(static_cast<Type>((static_cast<IntermediateType>(value) * other.value) >> fraction), raw_value_construct_tag{});
        }
    }

    constexpr inline fixed_num operator*=(const fixed_num& other) noexcept
    {
        if(rounding)
        {
            auto _value = (static_cast<IntermediateType>(value) * other.value) / (fraction_multiplier / 2);
            value = static_cast<Type>(_value + (_value % 2));
        }
        else
        {
            auto _value = (static_cast<IntermediateType>(value) * other.value) >> fraction;
            value = static_cast<Type>(_value);
        }
        return *this;
    }

    constexpr inline fixed_num operator*=(const std::integral auto& val) noexcept
    {
        value *= val;
        return *this;
    }

    constexpr inline fixed_num operator/(const fixed_num& other) const
    {
        if(other.value == 0)
            throw divide_by_zero();

        if(rounding)
        {
            auto _value = ((static_cast<IntermediateType>(value) << fraction) * 2) / other.value;
            return fixed_num(static_cast<Type>(_value + (_value % 2)), raw_value_construct_tag{});
        }
        else
        {
            return fixed_num(static_cast<Type>((static_cast<IntermediateType>(value) << fraction) / other.value), raw_value_construct_tag{});
        }
    }

    constexpr inline fixed_num operator/=(const fixed_num& other)
    {
        if(other.value == 0)
            throw divide_by_zero();

        if(rounding)
        {
            auto _value = ((static_cast<IntermediateType>(value) << fraction) * 2) / other.value;
            value = static_cast<Type>(_value + (_value % 2));
        }
        else
        {
            value = static_cast<Type>((static_cast<IntermediateType>(value) << fraction) / other.value);
        }
        return *this;
    }

    constexpr inline fixed_num operator/=(const std::integral auto& val)
    {
        if(val == 0)
            throw divide_by_zero();

        value /= val;
        return *this;
    }

    constexpr inline fixed_num operator%(const fixed_num& other) const noexcept
    {
        return fixed_num(value % other.value, raw_value_construct_tag{});
    }

    constexpr inline fixed_num operator%=(const fixed_num& other) noexcept
    {
        value %= other.value;
        return *this;
    }

    constexpr inline fixed_num operator-() const noexcept
    {
        return fixed_num(-value, raw_value_construct_tag{});
    }

    inline fixed_num operator++() noexcept
    {
        value += self_add_value;
        return *this;
    }

    inline fixed_num operator--() noexcept
    {
        value -= self_add_value;
        return *this;
    }

    constexpr inline bool operator==(const fixed_num& other) const noexcept
    {
        auto div = value - other.value;
        return div <= compare_epsilon_v && div >= -compare_epsilon_v;
    }

    constexpr inline bool operator!=(const fixed_num& other) const noexcept
    {
        return !(*this == other);
    }

    constexpr inline bool operator<(const fixed_num& other) const noexcept
    {
        auto div = value - other.value;
        return div < -compare_epsilon_v;
    }

    constexpr inline bool operator>(const fixed_num& other) const noexcept
    {
        auto div = value - other.value;
        return div > compare_epsilon_v;
    }

    constexpr inline bool operator>=(const fixed_num& other) const noexcept
    {
        return !(*this < other);
    }

    constexpr inline bool operator<=(const fixed_num& other) const noexcept
    {
        return !(*this > other);
    }

    /* strict compare */

    constexpr inline bool strict_eq(const fixed_num& other) const noexcept
    {
        return value == other.value;
    }

    constexpr inline bool strict_ne(const fixed_num& other) const noexcept
    {
        return value != other.value;
    }

    constexpr inline bool strict_gt(const fixed_num& other) const noexcept
    {
        return value > other.value;
    }

    constexpr inline bool strict_lt(const fixed_num& other) const noexcept
    {
        return value < other.value;
    }

    constexpr inline bool strict_gt_eq(const fixed_num& other) const noexcept
    {
        return value >= other.value;
    }

    constexpr inline bool strict_lt_eq(const fixed_num& other) const noexcept
    {
        return value <= other.value;
    }

    /* convert functions */

    template <unsigned int _fraction, typename T, typename std::enable_if_t<(_fraction > fraction), T*> = nullptr>
    static constexpr inline fixed_num from_fixed_num_value(T inner_value) noexcept
    {
        return rounding ?
                   fixed_num(static_cast<Type>(inner_value / (T(1) << (_fraction - fraction)) + (inner_value / (T(1) << (_fraction - fraction - 1)) % 2)), raw_value_construct_tag{}) :
                   fixed_num(static_cast<Type>(inner_value / (T(1) << (_fraction - fraction))), raw_value_construct_tag{});
    }

    template <unsigned int _fraction, typename T, typename std::enable_if_t<(_fraction <= fraction), T*> = nullptr>
    static constexpr inline fixed_num from_fixed_num_value(T inner_value) noexcept
    {
        return fixed_num(static_cast<Type>(inner_value * (T(1) << (fraction - _fraction))), raw_value_construct_tag{});
    }

    static constexpr inline fixed_num from_inner_value(Type inner_value) noexcept
    {
        return fixed_num(inner_value, raw_value_construct_tag{});
    }

private:
    Type value;

    static constexpr Type self_add_value = Type(1) << fraction;
    static constexpr Type compare_epsilon_v = nearly_compare_epsilon().value;
};

using fixed32 = fixed_num<int32_t, int64_t, 16, false>;

inline fixed32 operator""_f32(unsigned long long val)
{
    return fixed32(val);
}

inline fixed32 operator""_f32(long double val)
{
    return fixed32(val);
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator+(const fixed_num<T, I, f, r>& fp, const std::integral auto& val) noexcept
{
    return fixed_num<T, I, f, r>(fp) += val;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator+(const std::integral auto& val, const fixed_num<T, I, f, r>& fp) noexcept
{
    return fixed_num<T, I, f, r>(fp) += val;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator-(const fixed_num<T, I, f, r>& fp, const std::integral auto& val) noexcept
{
    return fixed_num<T, I, f, r>(fp) -= val;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator-(const std::integral auto& val, const fixed_num<T, I, f, r>& fp) noexcept
{
    return fixed_num<T, I, f, r>(val) -= fp;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator*(const fixed_num<T, I, f, r>& fp, const std::integral auto& val) noexcept
{
    return fixed_num<T, I, f, r>(fp) *= val;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator*(const std::integral auto& val, const fixed_num<T, I, f, r>& fp) noexcept
{
    return fixed_num<T, I, f, r>(fp) *= val;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator/(const fixed_num<T, I, f, r>& fp, const std::integral auto& val)
{
    return fixed_num<T, I, f, r>(fp) /= val;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator/(const std::integral auto& val, const fixed_num<T, I, f, r>& fp)
{
    return fixed_num<T, I, f, r>(val) /= fp;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator%(const fixed_num<T, I, f, r>& fp, const std::integral auto& val) noexcept
{
    return fixed_num<T, I, f, r>(fp) %= val;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> operator%(const std::integral auto& val, const fixed_num<T, I, f, r>& fp) noexcept
{
    return fixed_num<T, I, f, r>(val) %= fp;
}

/**
 * @brief Parse fixed point number from string, for angel script.
 *
 * @param start the beginning pos of the pointer.
 * @param stop the beginning pos of the pointer.
 * @param out tge output fixed point number
 * @return the pos of the pointer.
 */
const char* parse(const char* start, const char* stop, fixed32& out);

#endif // FIXED32_FIXED_HPP
