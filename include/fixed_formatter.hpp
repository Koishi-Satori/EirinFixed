#ifndef FIXED32_FIXED_FORMATTER_HPP
#define FIXED32_FIXED_FORMATTER_HPP

#include <papilio/format.hpp>
#include <papilio/core.hpp>
#include <fixed.hpp>

namespace papilio
{
template <typename T, typename I, unsigned int F, bool R, typename CharT>
class formatter<fixed_num<T, I, F, R>, CharT> : public std_formatter_base
{
    using my_base = std_formatter_base;

public:
    template <typename ParseContext>
    auto parse(ParseContext& ctx)
        -> typename ParseContext::iterator
    {
        using namespace std::literals;

        std_formatter_parser<ParseContext, false> parser;
        auto [data, it] = parser.parse(ctx, U"fgdXxBbo?"sv);
        if(data.fill == U'\0')
            data.fill = U' ';
        if(data.type == U'\0')
            data.type = U'g';
        my_base::data() = data;

        return it;
    }

    template <typename FormatContext>
    auto format(const fixed_num<T, I, F, R>& fp, FormatContext& ctx) const
        -> typename FormatContext::iterator
    {
        using context_t = format_context_traits<FormatContext>;

        // Internal representation for debugging
        if(data().type == U'?')
        {
            using namespace std::literals;

            std_formatter_data fmt_data = data();
            fmt_data.use_locale = false;
            fmt_data.type = U'x';

            int_formatter<T, CharT> fmt;
            fmt.set_data(fmt_data);
            return fmt.format(fp.inner_value(), ctx);
        }

        bool int_only = false;
        bool uppercase = false;
        int base = 10;

        switch(data().type)
        {
        case U'X':
            uppercase = true;
            [[fallthrough]];
        case U'x':
            int_only = true;
            base = 16;
            break;

        case U'd':
            int_only = true;
            base = 10;
            break;

        case U'o':
            int_only = true;
            base = 10;
            break;

        case U'B':
            uppercase = true;
            [[fallthrough]];
        case U'b':
            int_only = true;
            base = T(16);
            break;
        }

        T value = fp.inner_value();
        bool neg = signbit(fp);

        const T int_part = fp.integral_part();
        const T frac_part = fp.fractional_part();

        std::size_t used = 0;

        CharT int_buf[sizeof(T) * 8];
        CharT* int_buf_end = reverse_copy_int(int_buf, int_part, base, uppercase);
        used += static_cast<std::size_t>(int_buf_end - int_buf);

        switch(data().sign)
        {
        default:
        case format_sign::negative:
        case format_sign::default_sign:
            if(neg)
                ++used;
            break;

        case format_sign::positive:
        case format_sign::space:
            ++used;
            break;
        }

        if(int_only)
        {
            if(data().alternate_form)
                used += alt_prefix_width(base);

            auto [left, right] = data().fill_zero ?
                                     std::make_pair<std::size_t, std::size_t>(0, 0) :
                                     fill_size(used);

            fill(ctx, left);

            context_t::advance_to(
                ctx, copy_sign(ctx.out(), neg)
            );

            if(data().alternate_form && base != 10)
            {
                context_t::append(ctx, '0');
                switch(base)
                {
                case 16:
                    context_t::append(ctx, uppercase ? 'X' : 'x');
                    break;

                case 2:
                    context_t::append(ctx, uppercase ? 'B' : 'b');
                    break;

                default:
                case 10:
                case 8:
                    break;
                }
            }

            if(data().fill_zero)
            {
                if(used < data().width)
                {
                    context_t::append(
                        ctx,
                        static_cast<CharT>('0'),
                        data().width - used
                    );
                }
            }

            context_t::advance_to(
                ctx, std::reverse_copy(int_buf, int_buf_end, ctx.out())
            );

            fill(ctx, right);
        }
        else
        {
            T frac_val = fp.fractional_part();
            bool has_frac_part = frac_val != 0;

            CharT float_buf[sizeof(T) * 8];
            CharT* float_buf_end = float_buf;

            unsigned int precision = data().precision;
            if(precision == 0)
                precision = 6;
            unsigned int used_prec = 0;

            if(has_frac_part)
            {
                used += 1; // decimal point

                T divisor = static_cast<T>(1) << F;

                const auto& digits = uppercase ? digit_map_upper : digit_map_lower;

                for(unsigned int i = 0; i < F; ++i)
                {
                    if(i >= precision)
                    {
                        break;
                    }

                    if(frac_val == 0)
                    {
                        break;
                    }
                    if(divisor % base == 0)
                    {
                        divisor /= base;
                    }
                    else
                    {
                        frac_val *= base;
                    }

                    auto digit = (frac_val / divisor) % base;
                    *float_buf_end = digits[digit];
                    ++float_buf_end;
                    frac_val %= divisor;
                    ++used_prec;
                }

                used += static_cast<std::size_t>(float_buf_end - float_buf);
            }

            unsigned int zeros_to_append = data().type != 'g' ? precision - used_prec : 0;
            used += zeros_to_append;

            auto [left, right] = data().fill_zero ?
                                     std::make_pair<std::size_t, std::size_t>(0, 0) :
                                     fill_size(used);

            fill(ctx, left);

            if(data().fill_zero)
            {
                if(used < data().width)
                {
                    context_t::append(
                        ctx,
                        static_cast<CharT>('0'),
                        data().width - used
                    );
                }
            }

            context_t::advance_to(
                ctx, copy_sign(ctx.out(), neg)
            );
            context_t::advance_to(
                ctx, std::reverse_copy(int_buf, int_buf_end, ctx.out())
            );

            if(has_frac_part)
            {
                context_t::append(ctx, static_cast<CharT>('.'));
                context_t::advance_to(
                    ctx, std::copy(float_buf, float_buf_end, ctx.out())
                );
                context_t::append(
                    ctx,
                    static_cast<CharT>('0'),
                    zeros_to_append
                );
            }

            fill(ctx, right);
        }

        return ctx.out();
    }

private:
    template <typename OutputIt>
    OutputIt copy_sign(OutputIt out, bool neg) const
    {
        switch(data().sign)
        {
        default:
        case format_sign::negative:
        case format_sign::default_sign:
            if(neg)
            {
                *out = static_cast<CharT>('-');
                ++out;
            }
            break;

        case format_sign::positive:
            *out = static_cast<CharT>(neg ? '-' : '+');
            ++out;
            break;

        case format_sign::space:
            *out = static_cast<CharT>(neg ? '-' : ' ');
            ++out;
            break;
        }

        return out;
    }

    // Get width of the prefix of alternate form
    static std::size_t alt_prefix_width(int base) noexcept
    {
        switch(base)
        {
        default:
        case 10:
            return 0;

        case 2:
        case 16:
            return 2; // "0b", "0B", "0x" and "0X"

        case 8:
            return 1; // "o"
        }
    }

    template <typename OutputIt>
    OutputIt reverse_copy_int(OutputIt out, T val, int base, bool uppercase) const
    {
        const auto& digits = uppercase ? digit_map_upper : digit_map_lower;

        if constexpr(std::is_signed_v<T>)
        {
            do
            {
                const T digit = std::abs(val % static_cast<T>(base));
                *out = static_cast<CharT>(digits[digit]);
                ++out;
                val /= static_cast<T>(base);
            } while(val);
        }
        else
        {
            do
            {
                const T digit = val % static_cast<T>(base);
                *out = static_cast<CharT>(digits[digit]);
                ++out;
                val /= static_cast<T>(base);
            } while(val);
        }

        return out;
    }
};
} // namespace papilio

#endif
