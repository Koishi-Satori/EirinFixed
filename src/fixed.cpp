#include <cstdint>
#include <fixed.hpp>

bool f32_from_cstring(const char* str, size_t len, fixed32& fp) noexcept
{
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
    if (has_next()) {
        if(peek() == '-')
        {
            negative = true;
            ++pos;
        }
    }

    int32_t int_part = 0, dec_part = 0;
    // parse the integer part.
    while(has_next() && peek() != '.')
    {
        if(!isdigit(peek()))
        {
            return false;
        }
        int_part = int_part * 10 + (next() - '0');
    }
    // parse the decimal part.
    if(has_next())
    {
        ++pos;
        constexpr auto max_fraction = (1 << fixed32::precision) - 1;
        int32_t scale = 1, divisor = 1;
        while(has_next())
        {
            if(!isdigit(peek()))
            {
                return false;
            }
            if(dec_part > max_fraction / 10)
            {
                break;
            }
            auto digit = next() - '0';
            dec_part = dec_part * 10 + digit;
            divisor *= 10;
        }
        fp = fixed32::from_inner_value((int_part << fixed32::precision) + (dec_part << fixed32::precision) / divisor);
    }
    else
    {
        fp = fixed32::from_inner_value(int_part << fixed32::precision);
    }
    if(negative)
        fp = -fp;
    printf("parts: %d, %d\n", int_part, dec_part);
    return true;
}

template <typename CharT, class Traits>
std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, fixed32& fp) noexcept
{
}

const char* parse(const char* start, const char* stop, fixed32& out)
{
    size_t pos;
    auto peek = [&]() -> char
    {
        return start[pos];
    };
    auto next = [&]() -> char
    {
        return start[pos++];
    };
    auto has_next = [&]() -> bool
    {
        return pos < stop - start;
    };

    int32_t int_part, dec_part;
    bool negative = false;

    if(!has_next())
        return start;
    if(peek() == '-')
    {
        negative = true;
        ++pos;
    }
    // not finish yet.
    return stop;
}
