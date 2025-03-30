#ifndef EIRIN_FIXED_FIXED_EXT_ANGEL_SCRIPT_HPP
#define EIRIN_FIXED_FIXED_EXT_ANGEL_SCRIPT_HPP

#include <fixed.hpp>

namespace eirin
{
/**
     * @brief Parse fixed point number from string, for angel script.
     *
     * @param start the beginning pos of the pointer.
     * @param stop the end pos of the pointer.
     * @param out tge output fixed point number
     * @return the pos of the pointer.
     */
const char* parse(const char* start, const char* stop, fixed32& out);
} // namespace eirin


#endif
