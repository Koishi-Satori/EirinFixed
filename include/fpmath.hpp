#ifndef EIRIN_FIXED_MATH_HPP
#define EIRIN_FIXED_MATH_HPP

#include <fixed.hpp>
#include <stdexcept>

namespace eirin
{
constexpr inline fixed32 f32_max = fixed32::from_internal_value(0x7FFFFFFF);
constexpr inline fixed32 f32_min = fixed32::from_internal_value(0x80000000);
constexpr inline fixed64 f64_max = fixed64::from_internal_value(0x7FFFFFFFFFFFFFFF);
constexpr inline fixed64 f64_min = fixed64::from_internal_value(0x8000000000000000);

template <fixed_point T>
EIRIN_ALWAYS_INLINE constexpr inline T max_value() noexcept
{
    return std::numeric_limits<T>::max();
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> ceil(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto frac_mult = T(1) << f;
    auto value = fp.internal_value();
    if(value > 0)
        value += frac_mult - 1;
    // overflow check.
    if(value < 0)
        return fixed::from_internal_value(fp.internal_value() / frac_mult * frac_mult + frac_mult - 1);
    return fixed::from_internal_value(value / frac_mult * frac_mult);
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> floor(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto frac_mult = T(1) << f;
    auto value = fp.internal_value();
    auto neg = value < 0;
    if(value < 0)
        value -= frac_mult - 1;
    // underflow check.
    if(neg && value > 0)
        return fixed::from_internal_value(fp.internal_value() / frac_mult * frac_mult - frac_mult + 1);
    return fixed::from_internal_value(value / frac_mult * frac_mult);
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> trunc(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto frac_mult = T(1) << f;
    return fixed::from_internal_value(fp.internal_value() / frac_mult * frac_mult);
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> round(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    auto frac_mult = T(1) << f;
    auto value = fp.internal_value() / (frac_mult / 2);
    return fixed::from_internal_value((value / 2 + (value % 2)) << f);
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> abs(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    auto value = fp.internal_value();
    return fixed::from_internal_value(value < 0 ? -value : value);
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> min(fixed_num<T, I, f, r> a, fixed_num<T, I, f, r> b) noexcept
{
    auto a_i = a.internal_value();
    auto b_i = b.internal_value();
    return a_i < b_i ? a : b;
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> max(fixed_num<T, I, f, r> a, fixed_num<T, I, f, r> b) noexcept
{
    auto a_i = a.internal_value();
    auto b_i = b.internal_value();
    return a_i > b_i ? a : b;
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> sqrt(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    // test if T is int32_t, if so, we can use the fast sqrt algorithm.
    // if not, using newton method.
    if constexpr(std::is_same_v<T, int32_t>)
    {
        uint64_t t, q = 0, b = 0x40000000UL, v;
        if constexpr(f > 16)
        {
            constexpr auto move = f - 16;
            v = fp.internal_value() >> move;
        }
        else
        {
            constexpr auto move = 16 - f;
            v = fp.internal_value() << move;
        }

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
            return fixed::from_internal_value(static_cast<T>(q >> 8));
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
                return fixed::from_internal_value(static_cast<T>(q >> 7));
            }
            v <<= 1;
            b >>= 1;
        }
        return fixed::from_internal_value(static_cast<T>(q >> 8));
    }
    else
    {
        if(fp < fixed(0))
            return fixed(-1);
        if(fp == fixed(0))
            return fixed(0);
        const T val = fp.internal_value();
        const auto exponent = detail::find_msb(val);

        const auto init_value = fixed::get_sqrt_init_value(exponent);
        auto x = fixed::from_internal_value(init_value);

        auto eps = fixed::epsilon();
        for(int i = 0; i < 5; ++i)
        {
            x = (x + fp / x) / 2;
            if(abs(fp - x * x) < eps)
                break;
        }
        return x;
    }
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> sin(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    auto x = fixed(fp);
    x %= fixed::double_pi();
    x /= fixed::pi_2();
    constexpr auto fp1 = fixed(1);
    constexpr auto fp2 = fixed(2);

    if(x < fixed(0))
        x += fixed(4);

    auto negative = false;
    if(x > fp2)
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
    if(x > fp1)
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
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> cos(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    return sin(fp.internal_value() > 0 ? fp - (fixed::double_pi() - fixed::pi_2()) : fp + fixed::pi_2());
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> tan(fixed_num<T, I, f, r> fp)
{
    using fixed = fixed_num<T, I, f, r>;
    auto cosx = cos(fp);
    if(abs(cosx).internal_value() > 1)
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
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> atan(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto a = fixed::template from_fixed_num_value<16>(0x3985); // 0.2247
    constexpr auto b = fixed::template from_fixed_num_value<16>(0x10F9); // 0.0663
    return fixed::pi_4() * fp - fp * (abs(fp) - fixed(1)) * (a - b * abs(fp));
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> cbrt(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    auto x = (fixed(fp) + 2) / 3;
    auto iter_count = 0;
    constexpr auto precision = fixed::nearly_compare_epsilon() * 2;

    while(abs(fp - (x * x * x)) >= precision && iter_count <= 200)
    {
        x = (fp / (x * x) + x * 2) / 3;
        ++iter_count;
    }
    return x;
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> log2(fixed_num<T, I, f, r> fp)
{
    using fixed = fixed_num<T, I, f, r>;

    // This implementation is based on Clay. S. Turner's fast binary logarithm[1].
    // For some unknown reason, the GCC/Clang optimize this code to a very fast implementation,
    // when using '-O2' optimization and the fixed point number is a constant or literal,
    // it is even faster than the standard library's floating-point log2 function.
    // However, '-O3' optimization will not optimize this code to a fast implementation in the
    // same situation.
    // To be specific, the third loop(the for loop) will be optimized to the next assembly code:
    // ```
    // .L4:
    //         imul    rax, rax
    //         mov     rdi, rax
    //         shr     rax, 16
    //         cmp     rax, 131071
    //         jbe     .L3
    //         mov     rax, rdi
    //         add     rsi, rcx
    //         shr     rax, 17
    // ```
    // The .L3 code segments is the main function code, and the .L4 code segments is the for loop code.
    // The assembly code of '-O3' optimization is to expand the for loop code into tons of code segments,
    // which makes the performance of the code much slower than '-O2' optimization.
    // I prefer this situation is abnormal, but I don't know why.
    T b = 1u << (f - 1), y = 0, x = fp.internal_value();
    if(fp <= fixed(0))
        throw std::domain_error("error fp domain.");

    while(x < (static_cast<T>(1u) << f))
    {
        x <<= 1;
        y -= (static_cast<T>(1u) << f);
    }

    while(x >= (static_cast<T>(2u) << f))
    {
        x >>= 1;
        y += (static_cast<T>(1u) << f);
    }

    I z = x;
    for(size_t i = 0; i < f; ++i)
    {
        z = (z * z) >> f;
        if(z >= (static_cast<T>(2u) << f))
        {
            z >>= 1;
            y += b;
        }
        b >>= 1;
    }

    return fixed::from_internal_value(y);
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> log(fixed_num<T, I, f, r> fp)
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto log2_e = fixed::template from_fixed_num_value<60>(0x171547652B82FE00ll);
    return log2(fp) / log2_e;
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> log10(fixed_num<T, I, f, r> fp)
{
    using fixed = fixed_num<T, I, f, r>;
    constexpr auto log2_10 = fixed::template from_fixed_num_value<60>(0x35269E12F346E200ll);
    return log2(fp) / log2_10;
}

template <typename T, typename I, unsigned int f, bool r, std::integral E>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> pow(fixed_num<T, I, f, r> b, E e) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    if(b == fixed(0))
    {
        if(e == 0)
            return fixed(1);
        return fixed(0);
    }

    auto res = fixed(1);
    if(e < 0)
    {
        for(auto i = b; e != 0; e /= 2, i *= i)
        {
            if(e % 2 != 0)
                res /= i;
        }
    }
    else
    {
        for(auto i = b; e != 0; e /= 2, i *= i)
        {
            if(e % 2 != 0)
                res *= i;
        }
    }
    return res;
}

namespace detail
{
    template <typename T, typename I, unsigned int f, bool r>
    EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> exp_expand(fixed_num<T, I, f, r> fp) noexcept
    {
        using fixed = fixed_num<T, I, f, r>;
        // the integer part of the input fixed point number.
        const T x_int = fp.internal_value() / (I(1) << f);
        fp -= x_int;

        constexpr auto a = fixed::template from_fixed_num_value<63>(0x01C798ECC0CBC856ll); // 1.3903728105644451e-2
        constexpr auto b = fixed::template from_fixed_num_value<63>(0x04745859810836DAll); // 3.4800571158543038e-2
        constexpr auto c = fixed::template from_fixed_num_value<63>(0x15CFBB5C306F85F3ll); // 1.7040197373796334e-1
        constexpr auto d = fixed::template from_fixed_num_value<63>(0x3FE26186C531F98Ell); // 4.9909609871464493e-1
        constexpr auto e = fixed::template from_fixed_num_value<63>(0x40014D4407008BB0ll); // 1.0000794567422495
        constexpr auto _f = fixed::template from_fixed_num_value<63>(0x7FFFF686446F1B43ll); // 9.9999887043019773e-1
        return pow(fixed::e(), x_int) * (_f + fp * (e + fp * (d + fp * (c + fp * (b + fp * a)))));
    }
} // namespace detail

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> exp(fixed_num<T, I, f, r> fp) noexcept
{
    using fixed = fixed_num<T, I, f, r>;

    if(fp == fixed(0))
        return fixed(1);
    if(fp < fixed(0))
        return fixed(1) / detail::exp_expand(-fp);
    return detail::exp_expand(fp);
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> pow(fixed_num<T, I, f, r> b, fixed_num<T, I, f, r> e) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    if(b == fixed(0))
    {
        if(e == fixed(0))
            return fixed(1);
        return fixed(0);
    }

    // the integer part of the input exponent.
    const T e_int = e.internal_value() / (I(1) << f);
    e -= e_int;

    return pow(b, e_int) * exp(e * log(b));
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> fmod(fixed_num<T, I, f, r> a, fixed_num<T, I, f, r> b) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    return a - b * floor(a / b);
}

template <typename T, typename I, unsigned int f, bool r>
EIRIN_ALWAYS_INLINE constexpr inline fixed_num<T, I, f, r> modf(fixed_num<T, I, f, r> fp, fixed_num<T, I, f, r>& int_part) noexcept
{
    using fixed = fixed_num<T, I, f, r>;
    int_part = floor(fp);
    return fp - int_part;
}
} // namespace eirin

#endif // EIRIN_FIXED_MATH_HPP
