#ifndef EIRIN_FIXED_EIRIN_HPP
#define EIRIN_FIXED_EIRIN_HPP

// IWYU pragma: begin_exports

#include <eirin/fixed.hpp>
#include <eirin/math.hpp>
#include <eirin/parse.hpp>
#include <eirin/macro.hpp>
#include <eirin/numbers.hpp>

// IWYU pragma: end_exports

#define EIRIN_FIXED_MAJOR_VERSION 1
#define EIRIN_FIXED_MINOR_VERSION 2
#define EIRIN_FIXED_PATCH_VERSION 0

#include <tuple>

namespace eirin
{
[[nodiscard]]
inline constexpr std::tuple<int, int, int> get_version() noexcept
{
    return {EIRIN_FIXED_MAJOR_VERSION, EIRIN_FIXED_MINOR_VERSION, EIRIN_FIXED_PATCH_VERSION};
}
} // namespace eirin

#endif
