#ifndef FIXED32_FIXED_FORMATTER_HPP
#define FIXED32_FIXED_FORMATTER_HPP

#include <papilio/format.hpp>
#include <papilio/core.hpp>
#include <fixed.hpp>

namespace papilio
{
template <typename T, typename I, unsigned int f, bool r, typename CharT>
class formatter<fixed_num<T, I, f, r>, CharT>
{
public:
    template <typename ParseContext>
    auto parse(ParseContext& ctx)
        -> typename ParseContext::iterator
    {
        using namespace std::literals;
        using parser_t = std_formatter_parser<ParseContext, false>;
        parser_t parser;
        auto [data, it] = parser.parse(ctx, U"sdfi"sv);
        return it;
    }

    template <typename FormatContext>
    auto format(const fixed_num<T, I, f, r>& fp, FormatContext& ctx) const
        -> typename FormatContext::iterator
    {
        using context_t = format_context_traits<FormatContext>;
        auto digits = m_lower ? "0123456789ABCDEF" : "0123456789abcdef";
        T divisor = T(1) << f;
        T base = m_scale == scale_dec ? T(10) : (m_scale == scale_hex ? T(16) : (m_scale == scale_oct ? T(8) : T(2)));
        auto value = fp.inner_value();
        auto int_part = value >> f;
        value %= divisor;
        std::array<char, 512> buffer;
        auto p = buffer.begin();
        auto put_char = [&](const char c)
        {
            context_t::append(ctx, c);
        };

        // outout inner value
        if(m_data.type == U'i')
        {
            context_t::format_to(
                ctx,
                PAPILIO_TSTRING_VIEW(CharT, "{}"),
                fp.inner_value()
            );
            return ctx.out();
        }

        // if not only ouput the decimal part, put the int part.
        if(m_data.type != U'f')
        {
            if(int_part == 0)
            {
                put_char('0');
            }
            else
            {
                while(int_part > 0)
                {
                    auto digit = int_part % base;
                    *p++ = digits[digit];
                    int_part /= base;
                    if(p == buffer.end())
                    {
                        while(p-- != buffer.begin())
                        {
                            put_char(*p);
                        }
                        put_char(*p);
                    }
                }
            }
            while(p-- != buffer.begin())
            {
                put_char(*p);
            }
            put_char(*p);
        }

        // if only output integral part, just return.
        if(m_data.type == U'd')
        {
            return ctx.out();
        }

        // output the dec part.
        if(value != 0)
        {
            put_char('.');
            for(unsigned int i = 0; i < f; ++i)
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

        return ctx.out();
    }

private:
    bool m_string = false;
    bool m_integral = false;
    bool m_decimal = false;
    bool m_inner = false;
    bool m_lower = false;
    int m_scale = 0;
    const int scale_dec = 0;
    const int scale_hex = 1;
    const int scale_oct = 2;
    const int scale_bin = 3;
    std_formatter_data m_data;
};
} // namespace papilio

#endif
