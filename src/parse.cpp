#include <cstdint>
#include <eirin/parse.hpp>

namespace eirin
{
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
        int32_t divisor = 1;
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

const char* parse(std::string_view str, fixed32& out)
{
    return parse(
        std::to_address(str.begin()),
        std::to_address(str.end()),
        out
    );
}
} // namespace eirin
