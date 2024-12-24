#ifndef FIXED_MATH_HPP
#define FIXED_MATH_HPP

#include <fixed.hpp>
#include <stdexcept>

constexpr inline fixed32 f32_max = fixed32::from_inner_value(0x7FFFFFFF);
constexpr inline fixed32 f32_min = fixed32::from_inner_value(0x80000000);

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> abs(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    return fixed::from_inner_value(abs(fp.inner_value()));
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> min(fixed_num<T, I, f, r> a, fixed_num<T, I, f, r> b) noexcept
{
    auto a_i = a.inner_value();
    auto b_i = b.inner_value();
    return a_i < b_i ? a : b;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> max(fixed_num<T, I, f, r> a, fixed_num<T, I, f, r> b) noexcept
{
    auto a_i = a.inner_value();
    auto b_i = b.inner_value();
    return a_i > b_i ? a : b;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> sqrt(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto move = (f > 16) ? (f - 16) : (16 - f);
    uint32_t t, q = 0, b = 0x40000000UL, v = (f > 16) ? (fp.inner_value() >> move) : (fp.inner_value() << move);

    // fix number sqrt using bit hack.
    if(v < 0x40000200)
    {
        while(b != 0x40)
        {
            t = q + b;
            if(v >= t)
            {
                v -= t;
                q = t + b;
            }
            v <<= 1;
            b >>= 1;
        }
        return fixed::from_inner_value(static_cast<T>(q >> 8));
    }
    while(b > 0x40)
    {
        t = q + b;
        if(v >= t)
        {
            v -= t;
            q = t + b;
        }
        if((r & 0x80000000) != 0)
        {
            q >>= 1;
            b >>= 1;
            v >>= 1;
            while(b > 0x20)
            {
                t = q + b;
                if(v >= t)
                {
                    v -= t;
                    q = t + b;
                }
                v <<= 1;
                b >>= 1;
            }
            return fixed::from_inner_value(static_cast<T>(q >> 7));
        }
        v <<= 1;
        b >>= 1;
    }
    return fixed::from_inner_value(static_cast<T>(q >> 8));
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> sin(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    auto x = fixed(fp);
    x %= fixed::double_pi();
    x /= fixed::pi_2();
    constexpr auto fp1 = fixed(1);
    constexpr auto fp2 = fixed(2);

    if(x.strict_lt(fixed(0)))
        x += fixed(4);

    auto negative = false;
    if(x.strict_gt(fp2))
    {
        negative = true;
        x -= fp2;
    }

    // we reconginzed the sin(x) = x when x is small enough.
    // sin(0.00015) = 0.000149999999437, so we use 0.0001 as the threshold.
    if(abs(fp) < fixed(0.00015))
        return negative ? -fp : fp;

    // reduce the range to [0, 1] due to sin is
    // symmetrical around PI / 2 in the domain [0, PI].
    if(x.strict_gt(fp1))
        x = fp2 - x;

    // we use tyler series to calculate sin(x).
    // n = 4 has enough precision.
    const auto x2 = x * x;
    constexpr auto a = fixed::pi() * fixed::pi() / 24;
    constexpr auto b = fixed::pi() * fixed::pi() / 80;
    constexpr auto c = fixed::pi() * fixed::pi() / 168;
    constexpr auto d = fixed::pi() * fixed::pi() / 288;
    auto res = fixed::pi() * x * (fp1 - a * x2 * (fp1 - b * x2 * (fp1 - c * x2 * (fp1 - d * x2)))) / 2;
    return negative ? -res : res;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> cos(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    return sin(fp.inner_value() > 0 ? fp - (fixed::double_pi() - fixed::pi_2()) : fp + fixed::pi_2());
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> tan(fixed_num<T, I, f, r> fp)
{
    using fixed = fixed_num<T, I, f, r>;
    auto cosx = cos(fp);
    if(cosx.inner_value() > 1)
        return sin(fp) / cosx;
    else
        throw std::domain_error("error fp domain.");
}

/**
 * @brief Arctangent function for fixed point number, using the fitting
 *        method from the paper "Efficient Approximations for the Arctangent Function".
 * @note reference paper: https://ieeexplore.ieee.org/document/1628884
 * 
 * @tparam T @see fixed_num
 * @tparam I @see fixed_num
 * @tparam f @see fixed_num
 * @tparam r @see fixed_num
 * @param fp the x of atan(x)
 * @return atan(x).
 */
template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> atan(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto a = fixed::template from_fixed_num_value<16>(0x3985); // 0.2247
    constexpr auto b = fixed::template from_fixed_num_value<16>(0x10F9); // 0.0663
    return fixed::pi_4() * fp - fp * (abs(fp) - fixed(1)) * (a - b * abs(fp));
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> cbrt(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    auto x = (fixed(fp) + 2) / 3;
    auto iter_count = 0;
    constexpr auto precision = fixed::nearly_compare_epsilon() * 2;

    while(abs(fp - (x * x * x)).strict_gt_eq(precision) && iter_count <= 200)
    {
        x = (fp / (x * x) + x * 2) / 3;
        ++iter_count;
    }
    return x;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> log2(fixed_num<T, I, f, r> fp) {
    using fixed = fixed_num<T, I, f, r>;

    // This implementation is based on Clay. S. Turner's fast binary logarithm[1].
    T b = 1u << (f - 1), y = 0, x = fp.inner_value();
    if(fp.strict_lt_eq(fixed(0)))
        throw std::domain_error("error fp domain.");

    while(x < (1u << f))
    {
        x <<= 1;
        y -= (1u << f);
    }

    while(x >= (2u << f))
    {
        x >>= 1;
        y += (1u << f);
    }

    I z = x;
    for(size_t i = 0; i < f; ++i)
    {
        z = (z * z) >> f;
        if(z >= (2u << f))
        {
            z >>= 1;
            y += b;
        }
        b >>= 1;
    }

    return fixed::from_inner_value(y);
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> log(fixed_num<T, I, f, r> fp)
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto log2_e = fixed::template from_fixed_num_value<30>(0x5C551D80ll); // 1.442695041
    return log2(fp) * log2_e;
}

template <typename T, typename I, unsigned int f, bool r>
constexpr inline fixed_num<T, I, f, r> log10(fixed_num<T, I, f, r> fp)
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto log2_10 = fixed::template from_fixed_num_value<30>(0x4D104D42ll); // 3.321928095
    return log2(fp) * log2_10;
}

#endif // FIXED_MATH_HPP
