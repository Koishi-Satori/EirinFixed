#ifndef EIRIN_FIXED_PARSE_HPP
#define EIRIN_FIXED_PARSE_HPP

#pragma once

#include "fixed.hpp"

namespace eirin
{
/**
 * @brief Parse fixed point number from string
 *
 * @param start the beginning pos of the pointer.
 * @param stop the end pos of the pointer.
 * @param out tge output fixed point number
 * @return the pos of the pointer.
 */
const char* parse(const char* start, const char* stop, fixed32& out);
} // namespace eirin


#endif
