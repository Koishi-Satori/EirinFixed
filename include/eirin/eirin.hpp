#ifndef EIRIN_FIXED_EIRIN_HPP
#define EIRIN_FIXED_EIRIN_HPP

#include <eirin/fixed.hpp>
#include <eirin/math.hpp>
#include <eirin/parse.hpp>
#include <eirin/macro.hpp>
#include <eirin/numbers.hpp>

#define EIRIN_FIXED_MAJOR_VERSION 1
#define EIRIN_FIXED_MINOR_VERSION 1
#define EIRIN_FIXED_PATCH_VERSION 1

#include <tuple>

namespace eirin
{
    inline constexpr std::tuple<int, int, int> get_version() noexcept
    {
        return {1, 1, 1};
    }
} // namespace eirin

#endif
