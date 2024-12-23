#ifndef FIXED_MATH_HPP
#define FIXED_MATH_HPP

#include <fixed.hpp>

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

#endif // FIXED_MATH_HPP
