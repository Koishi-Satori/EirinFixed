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
    if(has_next() && peek() == '-')
    {
        negative = true;
        next();
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
    if(has_next() && peek() == '.')
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
        fp = fixed32::from_internal_value((int_part << fixed32::precision) + (dec_part << fixed32::precision) / divisor);
    }
    else
    {
        fp = fixed32::from_internal_value(int_part << fixed32::precision);
    }
    if(negative)
        fp = -fp;
    return true;
}

const char* parse(const char* start, const char* stop, fixed32& out)
{
    size_t pos = 0;
    bool negative = false;
    auto peek = [&]() -> char
    {
        return *start;
    };
    auto next = [&]() -> char
    {
        ++pos;
        return *(start++);
    };
    auto has_next = [&]() -> bool
    {
        return *start != *stop;
    };

    if(has_next() && peek() == '-')
    {
        negative = true;
        next();
    }

    int32_t int_part = 0, dec_part = 0;
    // parse the integer part.
    while(has_next() && peek() != '.')
    {
        if(!isdigit(peek()))
        {
            return start -= pos;
        }
        int_part = int_part * 10 + (next() - '0');
    }
    // parse the decimal part.
    if(has_next() && peek() == '.')
    {
        next();
        constexpr auto max_fraction = (1 << fixed32::precision) - 1;
        int32_t scale = 1, divisor = 1;
        while(has_next())
        {
            if(!isdigit(peek()))
            {
                return start -= pos;
            }
            if(dec_part > max_fraction / 10)
            {
                break;
            }
            auto digit = next() - '0';
            dec_part = dec_part * 10 + digit;
            divisor *= 10;
        }
        out = fixed32::from_internal_value((int_part << fixed32::precision) + (dec_part << fixed32::precision) / divisor);
    }
    else
    {
        out = fixed32::from_internal_value(int_part << fixed32::precision);
    }
    if(negative)
        out = -out;
    return start;
}
