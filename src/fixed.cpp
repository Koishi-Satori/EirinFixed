#include <fixed.hpp>

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
