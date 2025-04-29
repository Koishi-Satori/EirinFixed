#ifndef EIRIN_FIXED_UTIL_HPP
#define EIRIN_FIXED_UTIL_HPP

#include <cmath>
#include <array>
#include <cstdio>

namespace eirin::util
{
    template<int N>
    struct fraction_bit_value
    {
        static constexpr double value = fraction_bit_value<N - 1>::value / 2;
    };
    template<>
    struct fraction_bit_value<0>
    {
        static constexpr double value = 0.5;
    };
    template<size_t ...N>
    static constexpr auto fraction_bit_eval(std::index_sequence<N...>)
    {
        constexpr auto fraction_bits = std::array{fraction_bit_value<N>::value...};
        return fraction_bits;
    }
    template<size_t N>
    static constexpr auto fraction_bits()
    {
        return fraction_bit_eval(std::make_index_sequence<N>());
    }
    
    // how to calc the constants:
    // we currently use int64_t, and the first bit is sign bit.
    // those math constants integral part is always < 4,
    // so we can use 2 bits of integral parts to represent.
    // and what left is 61 bits of fraction parts.
    // the i-th bit represents 2^-i, and add up those bits we got the fraction parts.
    template<typename I = int64_t>
    requires std::is_integral_v<I>
    inline constexpr I eval_value(double val, int fraction) noexcept
    {
        I int_part = static_cast<I>(val), res = I(0);
        constexpr auto fraction_bits_arr = fraction_bits<sizeof(I) * 8>();
        double frac_part = val - int_part;
        for(int i = 1; i <= fraction; ++i)
        {
            double cur_bit_value = fraction_bits_arr[i - 1];
            if(frac_part >= cur_bit_value)
            {
                res |= (1ll << (fraction - i));
                frac_part -= cur_bit_value;
            }
        }
        return res + (int_part << fraction);
    }

    inline constexpr long double cst_lst[] = {
        1.442695040888963407359924681001892137L,
        0.434294481903251827651128918916605082L,
        0.318309886183790671537767526745028724L,
        0.564189583547756286948079451560772586L,
        0.693147180559945309417232121458176568L,
        2.302585092994045684017991454684364208L,
        1.414213562373095048801688724209698079L,
        1.732050807568877293527446341505872367L,
        0.577350269189625764509148780501957456L,
        0.577215664901532860606512090082402431L,
        1.618033988749894848204586834365638118L
    };
    inline const char* csl_nme_lst[] = {
        "log2e",
        "log10e",
        "inv_pi",
        "inv_sqrtpi",
        "ln2",
        "ln10",
        "sqrt2",
        "sqrt3",
        "inv_sqrt3",
        "egamma",
        "phi"
    };

    inline constexpr size_t cst_lst_size = sizeof(cst_lst) / sizeof(long double);

    inline void print_constants()
    {
        for(size_t i = 0; i < cst_lst_size; ++i)
        {
            auto val = eval_value(cst_lst[i], 61);
            auto name = csl_nme_lst[i];
            // // log_2 e
// template <typename T>
// EIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> log2e_v()
// {
//     return T::template from_fixed_num_value<61>(0x2e2a8eca5705fc00ll);
// }
            printf("\n// %s\ntemplate <typename T>\nEIRIN_ALWAYS_INLINE constexpr enable_if_fixed<T> %s_v()\n", name, name);
            printf("{\n\treturn T::template from_fixed_num_value<61>(0x%lxll);\n}\n", val);
        }
    }
}

#endif
