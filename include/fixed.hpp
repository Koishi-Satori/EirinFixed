#ifndef EIRIN_FIXED_FIXED_HPP
#define EIRIN_FIXED_FIXED_HPP

#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ios>
#include <istream>
#include <stdexcept>
#include <type_traits>
#include <concepts>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int/cpp_int_config.hpp>
#include <boost/multiprecision/detail/standalone_config.hpp>
#include <boost/multiprecision/traits/std_integer_traits.hpp>
#include <papilio/format.hpp>
#include <macro.hpp>

namespace eirin
{

class divide_by_zero : public std::domain_error
{
public:
    divide_by_zero()
        : std::domain_error("Divide by zero.") {};
};

namespace detail
{
    template <typename T, unsigned int F, int E>
    struct sqrt_init_value
    {
        static constexpr T value() noexcept
        {
            constexpr int half_exp = E / 2;
            constexpr bool adjust = (E % 2) != 0;

            constexpr T base = static_cast<T>(1) << half_exp;
            constexpr T value = adjust ? (base | (base >> 1)) : base;

            return value << (F / 2 + F % 2);
        }
    };

    template <typename T, unsigned int F, typename S = std::make_index_sequence<sizeof(T) * 8>>
    struct sqrt_lookup_table;

    template <typename T, unsigned int F, size_t... I>
    struct sqrt_lookup_table<T, F, std::index_sequence<I...>>
    {
        static constexpr std::array<T, sizeof...(I)> generate() noexcept
        {
            return {{sqrt_init_value<T, F, I>::value()...}};
        }
    };

    template <typename T>
    constexpr int find_msb(T value) noexcept
    {
        if(value == 0)
            return -1;
        if constexpr(std::is_signed_v<T>)
        {
            using U = std::make_unsigned_t<T>;
            auto abs_v = value < 0 ? -value : value;
            return find_msb(static_cast<U>(abs_v));
        }
        else
        {
            int bits = 0;
            while(value >>= 1)
                ++bits;
            return bits;
        }
    }

    template <typename T>
    struct is_signed : public std::is_signed<T> {};

    template <>
    struct is_signed<boost::multiprecision::int128_type> : public std::true_type {};
} // namespace detail

template <typename Type, unsigned int fraction>
concept fixed_num_fraction = fraction > 0 && fraction <= sizeof(Type) * 8 - 1;

template <typename Type, typename IntermediateType>
concept fixed_num_size = sizeof(IntermediateType) > sizeof(Type);

template <typename Type, typename IntermediateType>
concept fixed_num_signness = detail::is_signed<IntermediateType>::value == detail::is_signed<Type>::value;

template <typename Type, typename IntermediateType, unsigned int fraction>
concept fixed_num_check = std::is_integral_v<Type> && fixed_num_fraction<Type, fraction> && fixed_num_size<Type, IntermediateType> && fixed_num_signness<Type, IntermediateType>;

template <int scale>
concept fixed_format_check_scale = requires {
    scale == 2 || scale == 8 || scale == 10 || scale == 16;
};

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
        : m_value(val) {};

public:
    inline fixed_num() noexcept = default;

    template <std::integral T>
    EIRIN_ALWAYS_INLINE constexpr inline explicit fixed_num(T val) noexcept
        : m_value(static_cast<Type>(val) << fraction){};

    template <std::floating_point T>
    EIRIN_ALWAYS_INLINE constexpr inline explicit fixed_num(T val) noexcept
        : m_value(static_cast<Type>(rounding ? (val >= 0.0) ? (val * fraction_multiplier * T{0.5}) : (val * fraction_multiplier - T{0.5}) : (val * fraction_multiplier))){};

    template <typename T, typename I, unsigned int f, bool r>
    EIRIN_ALWAYS_INLINE constexpr inline explicit fixed_num(fixed_num<T, I, f, r> fp) noexcept
        : m_value(from_fixed_num_value<f>(fp.internal_value()).internal_value())
    {}

    /**
        * @brief Get the inner value of the fixed number.
        * @note do not use unless you know what this function is and what are u doing.
        */
    EIRIN_ALWAYS_INLINE constexpr inline Type internal_value() const noexcept
    {
        return m_value;
    }

    /* constant defines */

    static constexpr fixed_num epsilon()
    {
        return from_fixed_num_value<64>(0x5000000000000ll);
    }

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

    EIRIN_ALWAYS_INLINE static constexpr inline Type signbit_mask() noexcept
    {
        return static_cast<Type>(1) << (sizeof(Type) * 8 - 1);
    }

    EIRIN_ALWAYS_INLINE friend constexpr inline bool signbit(const fixed_num& f) noexcept
    {
        if constexpr(std::is_signed_v<Type>)
            return f.m_value & signbit_mask();
        else // unsigned value
            return false;
    }

    EIRIN_ALWAYS_INLINE constexpr inline Type raw_integral_part() const noexcept
    {
        Type result = m_value;
        result &= ~signbit_mask(); // Remove signbit
        result >>= fraction; // Drop fractional part
        return result;
    }

    EIRIN_ALWAYS_INLINE constexpr inline Type integral_part() const noexcept
    {
        Type result = m_value;
        if(signbit(*this))
        {
            result = ~result;
            result += static_cast<Type>(1) << fraction;
        }
        result >>= fraction; // Drop fractional part

        return result;
    }

    EIRIN_ALWAYS_INLINE constexpr inline Type fractional_part() const noexcept
    {
        return m_value % (static_cast<Type>(1) << fraction);
    }

    /* operator override functions */

    fixed_num& operator=(const fixed_num&) noexcept = default;

    template <std::integral T>
    constexpr inline explicit operator T() const noexcept
    {
        return static_cast<T>(m_value >> fraction);
    }

    template <std::floating_point T>
    constexpr inline explicit operator T() const noexcept
    {
        return static_cast<T>(m_value) / fraction_multiplier;
    }

    constexpr inline fixed_num operator+(const fixed_num& other) const noexcept
    {
        return fixed_num(m_value + other.m_value, raw_value_construct_tag{});
    }

    constexpr inline fixed_num& operator+=(const fixed_num& other) noexcept
    {
        m_value += other.m_value;
        return *this;
    }

    constexpr inline fixed_num& operator+=(const std::integral auto& val) noexcept
    {
        m_value += static_cast<Type>(val) << fraction;
        return *this;
    }

    constexpr inline fixed_num operator-(const fixed_num& other) const noexcept
    {
        return fixed_num(m_value - other.m_value, raw_value_construct_tag{});
    }

    constexpr inline fixed_num& operator-=(const fixed_num& other) noexcept
    {
        m_value -= other.m_value;
        return *this;
    }

    constexpr inline fixed_num& operator-=(const std::integral auto& val) noexcept
    {
        m_value -= static_cast<Type>(val) << fraction;
        return *this;
    }

    constexpr inline fixed_num operator*(const fixed_num& other) const noexcept
    {
        if constexpr(rounding)
        {
            auto _value = (static_cast<IntermediateType>(m_value) * other.m_value) / (fraction_multiplier / 2);
            return fixed_num(static_cast<Type>(_value + (_value % 2)), raw_value_construct_tag{});
        }
        else
        {
            return fixed_num(static_cast<Type>((static_cast<IntermediateType>(m_value) * other.m_value) >> fraction), raw_value_construct_tag{});
        }
    }

    constexpr inline fixed_num& operator*=(const fixed_num& other) noexcept
    {
        if constexpr(rounding)
        {
            auto _value = (static_cast<IntermediateType>(m_value) * other.m_value) / (fraction_multiplier / 2);
            m_value = static_cast<Type>(_value + (_value % 2));
        }
        else
        {
            auto _value = (static_cast<IntermediateType>(m_value) * other.m_value) >> fraction;
            m_value = static_cast<Type>(_value);
        }
        return *this;
    }

    constexpr inline fixed_num& operator*=(const std::integral auto& val) noexcept
    {
        m_value *= val;
        return *this;
    }

    constexpr inline fixed_num operator/(const fixed_num& other) const
    {
        if(other.m_value == 0)
            throw divide_by_zero();

        if constexpr(rounding)
        {
            auto _value = ((static_cast<IntermediateType>(m_value) << fraction) * 2) / other.m_value;
            return fixed_num(static_cast<Type>(_value + (_value % 2)), raw_value_construct_tag{});
        }
        else
        {
            return fixed_num(static_cast<Type>((static_cast<IntermediateType>(m_value) << fraction) / other.m_value), raw_value_construct_tag{});
        }
    }

    constexpr inline fixed_num& operator/=(const fixed_num& other)
    {
        if(other.m_value == 0)
            throw divide_by_zero();

        if constexpr(rounding)
        {
            auto _value = ((static_cast<IntermediateType>(m_value) << fraction) * 2) / other.m_value;
            m_value = static_cast<Type>(_value + (_value % 2));
        }
        else
        {
            m_value = static_cast<Type>((static_cast<IntermediateType>(m_value) << fraction) / other.m_value);
        }
        return *this;
    }

    constexpr inline fixed_num& operator/=(const std::integral auto& val)
    {
        if(val == 0)
            throw divide_by_zero();

        m_value /= val;
        return *this;
    }

    constexpr inline fixed_num operator%(const fixed_num& other) const noexcept
    {
        return fixed_num(m_value % other.m_value, raw_value_construct_tag{});
    }

    constexpr inline fixed_num& operator%=(const fixed_num& other) noexcept
    {
        m_value %= other.m_value;
        return *this;
    }

    constexpr inline fixed_num operator-() const noexcept
    {
        return fixed_num(-m_value, raw_value_construct_tag{});
    }

    inline fixed_num operator++() noexcept
    {
        m_value += self_add_value;
        return *this;
    }

    inline fixed_num operator--() noexcept
    {
        m_value -= self_add_value;
        return *this;
    }

    constexpr friend bool operator==(const fixed_num& lhs, const fixed_num& rhs) noexcept = default;

    constexpr std::strong_ordering operator<=>(const fixed_num& rhs) const noexcept
    {
        return m_value <=> rhs.m_value;
    }

    /* nearly compare */

    constexpr inline bool nearly_eq(const fixed_num& other) const noexcept
    {
        auto div = m_value - other.m_value;
        return div >= -compare_epsilon_v && div <= compare_epsilon_v;
    }

    constexpr inline bool nearly_ne(const fixed_num& other) const noexcept
    {
        auto div = m_value - other.m_value;
        return div < -compare_epsilon_v || div > compare_epsilon_v;
    }

    constexpr inline bool nearly_gt(const fixed_num& other) const noexcept
    {
        auto div = m_value - other.m_value;
        return div > compare_epsilon_v;
    }

    constexpr inline bool nearly_lt(const fixed_num& other) const noexcept
    {
        auto div = m_value - other.m_value;
        return div < -compare_epsilon_v;
    }

    constexpr inline bool nearly_gt_eq(const fixed_num& other) const noexcept
    {
        return !(nearly_lt(other));
    }

    constexpr inline bool nearly_lt_eq(const fixed_num& other) const noexcept
    {
        return !(nearly_gt(other));
    }

    /* convert functions */

    template <unsigned int _fraction, typename T, typename std::enable_if_t<(_fraction > fraction), T*> = nullptr>
    EIRIN_ALWAYS_INLINE static constexpr inline fixed_num from_fixed_num_value(T inner_value) noexcept
    {
        return rounding ?
                   fixed_num(static_cast<Type>(inner_value / (T(1) << (_fraction - fraction)) + (inner_value / (T(1) << (_fraction - fraction - 1)) % 2)), raw_value_construct_tag{}) :
                   fixed_num(static_cast<Type>(inner_value / (T(1) << (_fraction - fraction))), raw_value_construct_tag{});
    }

    template <unsigned int _fraction, typename T, typename std::enable_if_t<(_fraction <= fraction), T*> = nullptr>
    EIRIN_ALWAYS_INLINE static constexpr inline fixed_num from_fixed_num_value(T inner_value) noexcept
    {
        return fixed_num(static_cast<Type>(inner_value * (T(1) << (fraction - _fraction))), raw_value_construct_tag{});
    }

    static constexpr fixed_num from_internal_value(Type internal_value) noexcept
    {
        return fixed_num(internal_value, raw_value_construct_tag{});
    }

    // generate lookup table for sqrt calc.
    static constexpr auto sqrt_init_table = detail::sqrt_lookup_table<Type, fraction>::generate();

    static constexpr Type get_sqrt_init_value(int exponent) noexcept
    {
        constexpr int max_exponent = sizeof(Type) * 8 - 1;
        const int clamped = std::clamp(exponent, 0, max_exponent);
        return sqrt_init_table[clamped];
    }

    template <typename CharT, class Traits>
    EIRIN_ALWAYS_INLINE inline std::basic_ostream<CharT, Traits>& print(std::basic_ostream<CharT, Traits>& os) const noexcept
    {
        auto uppercase = os.flags() & std::ios_base::uppercase;
        auto digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
        auto hex = os.flags() & std::ios_base::hex, dec = os.flags() & std::ios_base::dec, oct = os.flags() & std::ios_base::oct;
        Type divisor = static_cast<Type>(1) << fraction, base = static_cast<Type>(hex ? 16 : (dec ? 10 : (oct ? 8 : 2)));
        auto put_char = [&](const char c)
        {
            os.put(c);
        };

        auto value = m_value;
        Type int_part;
        if(value == minimum_value)
        {
            put_char('-');
            int_part = ~(value >> fraction) + 1;
            value = 0;
        }
        else
        {
            if(value < 0)
            {
                put_char('-');
                value = -value;
            }
            int_part = value >> fraction;
        }
        value %= divisor;
        std::array<char, 514> buffer;
        auto koishi = buffer.begin();

        if(int_part == 0)
        {
            put_char('0');
        }
        else
        {
            while(int_part > 0)
            {
                auto digit = int_part % base;
                *koishi++ = digits[digit];
                int_part /= base;
                if(koishi == buffer.end())
                {
                    while(koishi-- != buffer.begin())
                        put_char(*koishi);
                    koishi = buffer.begin();
                }
            }
        }
        while(koishi-- != buffer.begin())
            put_char(*koishi);

        if(value != 0)
        {
            put_char('.');
            for(unsigned int i = 0; i < fraction; ++i)
            {
                if(value == 0)
                {
                    break;
                }
                if(divisor % base == 0)
                {
                    divisor /= base;
                }
                else
                {
                    value *= base;
                }

                auto digit = (value / divisor) % base;
                put_char(digits[digit]);
                value %= divisor;
            }
        }

        os.flush();
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const fixed_num& fp) noexcept
    {
        return fp.print(os);
    }

private:
    Type m_value;

    static constexpr Type self_add_value = Type(1) << fraction;
    static constexpr Type compare_epsilon_v = nearly_compare_epsilon().m_value;
    static constexpr Type minimum_value = signbit_mask();
};

namespace detail
{
    template <typename T>
    struct is_fixed_point : std::false_type
    {};

    template <typename T, typename I, unsigned int f, bool r>
    struct is_fixed_point<fixed_num<T, I, f, r>> : public std::true_type
    {};

    template <typename CharT, typename T, typename I, unsigned int f, bool r>
    constexpr inline bool parse(const CharT* str, size_t len, fixed_num<T, I, f, r>& fp) noexcept
    {
        using fixed = fixed_num<T, I, f, r>;
        size_t pos = 0;
        bool negative = false;
        auto peek = [&]() -> CharT
        {
            return str[pos];
        };
        auto next = [&]() -> CharT
        {
            return str[pos++];
        };
        auto has_next = [&]() -> bool
        {
            return pos < len;
        };

        if(has_next() && peek() == '-')
        {
            negative = true;
            next();
        }

        int64_t int_part = 0, dec_part = 0;
        // parse the integer part.
        while(has_next() && peek() != '.')
        {
            if(!isdigit(peek()))
                return false;
            int_part = int_part * 10 + (next() - '0');
        }
        // parse the decimal part.
        if(has_next() && peek() == '.')
        {
            ++pos;
            constexpr auto max_fraction = ((int64_t)1 << fixed::precision) - 1;
            int64_t scale = 1, divisor = 1;
            while(has_next())
            {
                if(!isdigit(peek()))
                    return false;
                if(dec_part > max_fraction / 10)
                {
                    break;
                }
                auto digit = next() - '0';
                dec_part = dec_part * 10 + digit;
                divisor *= 10;
            }
            fp = fixed::from_internal_value((int_part << fixed::precision) + (dec_part << fixed::precision) / divisor);
        }
        else
        {
            fp = fixed::from_internal_value(int_part << fixed::precision);
        }
        if(negative)
            fp = -fp;
        return true;
    }
} // namespace detail

template <typename T>
inline constexpr bool is_fixed_point_v = detail::is_fixed_point<std::remove_cv_t<T>>::value;

template <typename T>
concept fixed_point = detail::is_fixed_point<std::remove_cv_t<T>>::value;

using fixed32 = fixed_num<int32_t, int64_t, 16, false>;
using fixed64 = fixed_num<int64_t, boost::multiprecision::int128_type, 32, false>;

inline fixed32 operator""_f32(unsigned long long val)
{
    return fixed32(val);
}

inline fixed32 operator""_f32(long double val)
{
    return fixed32(val);
}

constexpr fixed64 operator""_f64(const char* str, size_t len)
{
    fixed64 fp;
    detail::parse(str, len, fp);
    return fp;
}

template <char... chars>
constexpr fixed64 operator""_f64()
{
    auto len = sizeof...(chars);
    const char str[] = {chars...};
    fixed64 fp;
    detail::parse(str, len, fp);
    return fp;
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

template <typename CharT, class Traits, typename T, typename I, unsigned int F, bool R>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const fixed_num<T, I, F, R>& fp) noexcept
{
    return fp.print(os);
}

bool f32_from_cstring(const char* str, size_t len, fixed32& fp) noexcept;

template <typename T, typename I, unsigned int f, bool r>
bool fixed_from_cstring(const char* str, size_t len, fixed_num<T, I, f, r>& fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    size_t pos = 0;
    bool negative = false;
    auto peek = [&]() -> char
    {
        return str[pos];
    };
    auto next = [&]() -> char
    {
        return str[pos++];
    };
    auto has_next = [&]() -> bool
    {
        return pos < len;
    };
    if(has_next() && peek() == '-')
    {
        negative = true;
        next();
    }

    T int_part = T(0), dec_part = T(0);
    // parse the integer part.
    while(has_next() && peek() != '.')
    {
        if(!isdigit(peek()))
            break;
        int_part = int_part * 10 + (next() - '0');
    }
    // parse the decimal part.
    if(has_next() && peek() == '.')
    {
        next();
        constexpr auto max_fraction = (T(1) << f) - T(1);
        T scale = T(1), divisor = T(1);
        while(has_next())
        {
            if(dec_part > max_fraction / 10 || !isdigit(peek()))
                break;
            auto digit = next() - '0';
            dec_part = dec_part * 10 + digit;
            divisor *= 10;
        }
        fp = fixed::from_internal_value((int_part << f) + (dec_part << f) / divisor);
    }
    else
    {
        fp = fixed::from_internal_value(int_part << f);
    }
    if(negative)
        fp = -fp;
    return true;
}

inline fixed32 operator""_f32(const char* str)
{
    fixed32 fp;
    if(f32_from_cstring(str, std::strlen(str), fp))
        return fp;
    else
        throw std::runtime_error("failed converting string to fixed32");
}

inline fixed32 operator""_f32(const char* str, size_t len)
{
    fixed32 fp;
    if(f32_from_cstring(str, len, fp))
        return fp;
    else
        throw std::runtime_error("failed converting string to fixed32");
}

template <typename CharT, class Traits, typename T, typename I, unsigned int f, bool r>
std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, fixed_num<T, I, f, r>& fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    bool negative = false;
    auto peek = [&]() -> CharT
    {
        return is.peek();
    };
    auto next = [&]() -> CharT
    {
        return is.get();
    };
    auto has_next = [&]() -> bool
    {
        auto ch = is.peek();
        return !is.eof();
    };
    if(!has_next())
    {
        return is;
    }
    else if(peek() == '-')
    {
        negative = true;
        next();
    }

    T int_part = T(0), dec_part = T(0);
    // parse the integer part.
    while(has_next() && peek() != '.')
    {
        if(!isdigit(peek()))
            break;
        int_part = int_part * 10 + (next() - '0');
    }
    // parse the decimal part.
    if(has_next() && peek() == '.')
    {
        next();
        constexpr auto max_fraction = (T(1) << f) - T(1);
        T scale = T(1), divisor = T(1);
        while(has_next())
        {
            if(dec_part > max_fraction / 10 || !isdigit(peek()))
                break;
            auto digit = next() - '0';
            dec_part = dec_part * 10 + digit;
            divisor *= 10;
        }
        fp = fixed::from_internal_value((int_part << f) + (dec_part << f) / divisor);
    }
    else
    {
        fp = fixed::from_internal_value(int_part << f);
    }
    if(negative)
        fp = -fp;
    return is;
}

/**
     * @brief Parse fixed point number from string, for angel script.
     *
     * @param start the beginning pos of the pointer.
     * @param stop the end pos of the pointer.
     * @param out tge output fixed point number
     * @return the pos of the pointer.
     */
const char* parse(const char* start, const char* stop, fixed32& out);
} // namespace eirin

namespace std
{
template <typename T, typename I, unsigned int f, bool r>
struct numeric_limits<eirin::fixed_num<T, I, f, r>>
{
    using fixed_type = eirin::fixed_num<T, I, f, r>;

    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = false;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;

    static constexpr fixed_type radix = fixed_type(2);
    // 1 bit sign + fractions bits + integer bits
    static constexpr fixed_type digits = fixed_type(sizeof(T) * 8 - 1);
    // integer bits = sizeof(T) * 8 - 1 - f
    // fraction bits = f
    // digits10 = integer bits * log10(2) + fraction bits * log10(2)
    // we can use log10(2) = 643 / 2136
    static constexpr fixed_type digits10 = fixed_type(static_cast<T>((sizeof(T) * 8 - 1) * 643L / 2136));
    static constexpr fixed_type min_exponent = fixed_type(0);
    static constexpr fixed_type min_exponent10 = fixed_type(10);

    static constexpr float_denorm_style has_denorm = denorm_absent;
    static constexpr bool has_denorm_loss = false;

    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = false;

    static constexpr bool traps = true;
    static constexpr bool tinyness_before = false;
    static constexpr float_round_style round_style = round_toward_zero;

#define EIRIN_SHORT_IMPL(name)         \
    static constexpr fixed_type name() \
    {                                  \
        return fixed_type::name();     \
    }

#define EIRIN_DIRECT_IMPL(name, value) \
    static constexpr fixed_type name() \
    {                                  \
        return fixed_type(value);      \
    }

    EIRIN_SHORT_IMPL(epsilon)
    EIRIN_DIRECT_IMPL(round_error, 0)
    EIRIN_DIRECT_IMPL(infinity, 0)
    EIRIN_DIRECT_IMPL(quiet_NaN, 0)
    EIRIN_DIRECT_IMPL(signaling_NaN, 0)
    EIRIN_DIRECT_IMPL(denorm_min, 0)

    static constexpr fixed_type min() noexcept
    {
        if constexpr(std::numeric_limits<T>::is_specialized)
        {
            return fixed_type::from_internal_value(std::numeric_limits<T>::min());
        }
        else
        {
            constexpr fixed_type min_value = fixed_type::from_internal_value(fixed_type::signbit_mask());
            return min_value;
        }
    }

    static constexpr fixed_type max() noexcept
    {
        if constexpr(std::numeric_limits<T>::is_specialized)
        {
            return fixed_type::from_internal_value(std::numeric_limits<T>::max());
        }
        else
        {
            constexpr fixed_type max_value = fixed_type::from_internal_value(~fixed_type::signbit_mask());
            return max_value;
        }
    }

    static constexpr fixed_type lowest() noexcept
    {
        return min();
    }

#undef EIRIN_SHORT_IMPL
#undef EIRIN_DIRECT_IMPL
};
} // namespace std

#endif // EIRIN_FIXED_FIXED_HPP
