#ifndef EIRIN_FIXED_IO_FORMAT_HPP
#define EIRIN_FIXED_IO_FORMAT_HPP

#pragma once

#include <format>
#include <string>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <eirin/fixed.hpp>

namespace eirin::io
{

namespace detail
{

inline constexpr std::string_view digit_map_lower = "0123456789abcdef";
inline constexpr std::string_view digit_map_upper = "0123456789ABCDEF";

/**
 * @brief Convert an integer to a string in the given base.
 * @param val  the integer value (absolute value is used)
 * @param base the numeric base (2-16)
 * @param uppercase use uppercase letters for digits > 9
 * @return the string representation
 */
template <typename T, typename CharT>
std::basic_string<CharT> int_to_string(T val, int base, bool uppercase)
{
    const auto& digits = uppercase ? digit_map_upper : digit_map_lower;
    std::basic_string<CharT> result;

    if(val == 0)
    {
        result.push_back(static_cast<CharT>('0'));
        return result;
    }

    using U = std::make_unsigned_t<T>;
    U uval;
    if constexpr(std::is_signed_v<T>)
    {
        // Take absolute value safely (avoid overflow on most-negative)
        uval = val < 0 ? static_cast<U>(-(val + 1)) + U(1) : static_cast<U>(val);
    }
    else
    {
        uval = static_cast<U>(val);
    }

    while(uval > 0)
    {
        result.push_back(static_cast<CharT>(digits[uval % static_cast<U>(base)]));
        uval /= static_cast<U>(base);
    }
    std::reverse(result.begin(), result.end());
    return result;
}

/**
 * @brief Get the width of alternate-form prefix for a given base.
 */
inline constexpr std::size_t alt_prefix_width(int base) noexcept
{
    switch(base)
    {
    default:
    case 10: return 0;
    case 8:  return 1;
    case 2:
    case 16: return 2;
    }
}

} // namespace detail
} // namespace eirin::io

template <typename T, typename I, unsigned int F, bool R, typename CharT>
struct std::formatter<eirin::fixed_num<T, I, F, R>, CharT>
{
private:
    using fixed_type = eirin::fixed_num<T, I, F, R>;

    // parsed format-spec state
    CharT  fill_ch    = ' ';
    char   align_mode = '\0'; // '<' | '>' | '^' | '\0' -> default (right)
    char   sign_mode  = '-';  // '-' | '+' | ' '
    bool   alt_form   = false;
    bool   zero_pad   = false;
    int    width_val  = 0;
    int    prec       = -1;   // -1 means "not specified"
    char   fmt_type   = 'g';  // g G f F d x X o b B ?

public:
    // parse()
    constexpr auto parse(std::basic_format_parse_context<CharT>& ctx)
        -> typename std::basic_format_parse_context<CharT>::iterator
    {
        auto it  = ctx.begin();
        auto end = ctx.end();

        if(it == end)
            return it;

        {
            auto p = it;
            while(p != end && *p >= '0' && *p <= '9')
                ++p;
            if(p != end && *p == ':')
                it = p + 1;
        }

        if(it == end)
            return it;

        // [[fill]align]
        {
            auto nxt = it;
            ++nxt;
            if(nxt != end && (*nxt == '<' || *nxt == '>' || *nxt == '^'))
            {
                fill_ch    = *it;
                align_mode = *nxt;
                it += 2;
            }
            else if(*it == '<' || *it == '>' || *it == '^')
            {
                align_mode = *it;
                ++it;
            }
        }

        // [sign]
        if(it != end && (*it == '+' || *it == '-' || *it == ' '))
        {
            sign_mode = *it;
            ++it;
        }

        // [#]
        if(it != end && *it == '#')
        {
            alt_form = true;
            ++it;
        }

        // [0]
        if(it != end && *it == '0')
        {
            zero_pad = true;
            ++it;
        }

        // [width]
        if(it != end && *it >= '0' && *it <= '9')
        {
            width_val = 0;
            while(it != end && *it >= '0' && *it <= '9')
            {
                width_val = width_val * 10 + static_cast<int>(*it - '0');
                ++it;
            }
        }

        // [.precision]
        if(it != end && *it == '.')
        {
            ++it;
            prec = 0;
            while(it != end && *it >= '0' && *it <= '9')
            {
                prec = prec * 10 + static_cast<int>(*it - '0');
                ++it;
            }
        }

        // [type]
        if(it != end)
        {
            switch(*it)
            {
            case 'g': case 'G':
            case 'f': case 'F':
            case 'd':
            case 'x': case 'X':
            case 'o':
            case 'b': case 'B':
            case '?':
                fmt_type = *it;
                ++it;
                break;
            case '}':
                // MSVC may include the closing brace in format-spec
                break;
            default:
                throw std::format_error("invalid format type");
            }
        }

        return it;
    }
    // format()
    template <typename FormatContext>
    auto format(const fixed_type& fp, FormatContext& ctx) const
        -> typename FormatContext::iterator
    {
        auto out = ctx.out();

        // determine mode
        bool int_only   = false;
        bool uppercase  = false;
        int  base       = 10;

        switch(fmt_type)
        {
        case 'X': uppercase = true; [[fallthrough]];
        case 'x': int_only = true;  base = 16; break;
        case 'd': int_only = true;  base = 10; break;
        case 'o': int_only = true;  base = 8;  break;
        case 'B': uppercase = true; [[fallthrough]];
        case 'b': int_only = true;  base = 2;  break;
        case '?':
            return format_debug(fp, ctx);
        default:  // g, G, f, F
            if(fmt_type == 'G' || fmt_type == 'F')
                uppercase = true;
            break;
        }

        // sign & integer part
        bool neg     = signbit(fp);
        T    int_val = fp.integral_part();   // absolute value

        // digit string
        auto digit_str = eirin::io::detail::int_to_string<T, CharT>(int_val, base, uppercase);

        // prefix: sign + alt-prefix
        std::basic_string<CharT> prefix;
        append_sign(prefix, neg);
        if(alt_form && int_only && base != 10)
            append_alt_prefix(prefix, base, uppercase);

        // The "body" string
        std::basic_string<CharT> body;

        if(int_only)
        {
            if(zero_pad && width_val > 0)
            {
                std::size_t total = prefix.size() + digit_str.size();
                if(static_cast<int>(total) < width_val)
                {
                    std::size_t zeros = static_cast<std::size_t>(width_val) - total;
                    body = prefix + std::basic_string<CharT>(zeros, static_cast<CharT>('0')) + digit_str;
                }
                else
                {
                    body = prefix + digit_str;
                }
            }
            else
            {
                body = prefix + digit_str;
            }
            return write_aligned(out, body);
        }

        body = prefix + digit_str;
        {
            // fractional part
            T frac_val = fp.fractional_part();
            if constexpr(std::is_signed_v<T>)
            {
                if(frac_val < 0)
                    frac_val = -frac_val;
            }

            bool has_frac = (frac_val != 0);
            int  used_prec = 0;

            if(has_frac)
            {
                body.push_back(static_cast<CharT>('.'));

                const auto& digits = uppercase ?
                    eirin::io::detail::digit_map_upper :
                    eirin::io::detail::digit_map_lower;

                int eff_prec = (prec < 0) ? 6 : prec;

                T frac_bits = frac_val;
                // frac_bits represents value / 2^F.
                // Repeatedly multiply by 10 and extract the integer digit.
                for(unsigned int i = 0; i < F && used_prec < eff_prec; ++i)
                {
                    if(frac_bits == 0) break;

                    T shifted  = frac_bits * static_cast<T>(10);
                    T digit    = shifted >> F;
                    frac_bits  = shifted & ((static_cast<T>(1) << F) - static_cast<T>(1));

                    body.push_back(static_cast<CharT>(digits[static_cast<size_t>(digit)]));
                    ++used_prec;
                }
            }

            // trailing zeros for fixed-point types (f/F)
            if(fmt_type == 'f' || fmt_type == 'F')
            {
                int eff_prec = (prec < 0) ? 6 : prec;
                while(used_prec < eff_prec)
                {
                    if(used_prec == 0)
                        body.push_back(static_cast<CharT>('.'));
                    body.push_back(static_cast<CharT>('0'));
                    ++used_prec;
                }
            }
        }

        // width / fill / alignment
        return write_aligned(out, body);
    }

private:
    // format_debug - "?" type: show internal (raw) value in hex
    template <typename Ctx>
    auto format_debug(const fixed_type& fp, Ctx& ctx) const
    {
        auto out   = ctx.out();
        bool upper = false;

        T    int_val = fp.internal_value(); // raw bits
        bool neg     = false;

        using U = std::make_unsigned_t<T>;
        U uval;

        if constexpr(std::is_signed_v<T>)
        {
            if(int_val < 0)
            {
                neg = true;
                // safe absolute value
                uval = static_cast<U>(-(int_val + 1)) + U(1);
            }
            else
            {
                uval = static_cast<U>(int_val);
            }
        }
        else
        {
            uval = static_cast<U>(int_val);
        }

        std::basic_string<CharT> body;
        append_sign(body, neg);

        if(alt_form)
            append_alt_prefix(body, 16, upper);

        const auto& digits = eirin::io::detail::digit_map_lower;

        if(uval == 0)
        {
            body.push_back(static_cast<CharT>('0'));
        }
        else
        {
            std::basic_string<CharT> rev;
            while(uval > 0)
            {
                rev.push_back(static_cast<CharT>(digits[uval % 16U]));
                uval /= 16U;
            }
            std::reverse(rev.begin(), rev.end());
            body += rev;
        }

        return write_aligned(out, body);
    }
    // append_sign
    void append_sign(std::basic_string<CharT>& s, bool neg) const
    {
        switch(sign_mode)
        {
        default:
        case '-':
            if(neg) s.push_back(static_cast<CharT>('-'));
            break;
        case '+':
            s.push_back(static_cast<CharT>(neg ? '-' : '+'));
            break;
        case ' ':
            s.push_back(static_cast<CharT>(neg ? '-' : ' '));
            break;
        }
    }
    // append_alt_prefix
    void append_alt_prefix(std::basic_string<CharT>& s, int base, bool upper) const
    {
        switch(base)
        {
        case 2:
            s.push_back(static_cast<CharT>('0'));
            s.push_back(static_cast<CharT>(upper ? 'B' : 'b'));
            break;
        case 8:
            s.push_back(static_cast<CharT>('0'));
            break;
        case 16:
            s.push_back(static_cast<CharT>('0'));
            s.push_back(static_cast<CharT>(upper ? 'X' : 'x'));
            break;
        default:
            break;
        }
    }
    // write_aligned - apply width / fill / alignment and write to output
    template <typename OutputIt>
    OutputIt write_aligned(OutputIt out, const std::basic_string<CharT>& body) const
    {
        auto sz = body.size();
        if(width_val <= 0 || static_cast<std::size_t>(width_val) <= sz)
            return std::copy(body.begin(), body.end(), out);

        std::size_t pad_cnt = static_cast<std::size_t>(width_val) - sz;
        CharT pad_ch = zero_pad ? static_cast<CharT>('0') : fill_ch;

        char eff_align = (align_mode == '\0') ? '>' : align_mode;

        switch(eff_align)
        {
        case '<':
            out = std::copy(body.begin(), body.end(), out);
            out = std::fill_n(out, pad_cnt, pad_ch);
            break;

        case '^':
        {
            std::size_t left  = pad_cnt / 2;
            std::size_t right = pad_cnt - left;
            out = std::fill_n(out, left, pad_ch);
            out = std::copy(body.begin(), body.end(), out);
            out = std::fill_n(out, right, pad_ch);
            break;
        }

        case '>':
        default:
            out = std::fill_n(out, pad_cnt, pad_ch);
            out = std::copy(body.begin(), body.end(), out);
            break;
        }

        return out;
    }
};

#endif // EIRIN_FIXED_IO_FORMAT_HPP
