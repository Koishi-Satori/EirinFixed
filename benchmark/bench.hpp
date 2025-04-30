#include <eirin/fixed.hpp>

#pragma once

// prevent compiler optimization.
#ifdef EIRIN_FIXED_HAS_INT128
eirin::fixed64 f64_identity(eirin::fixed64 val);
#endif

eirin::fixed32 f32_identity(eirin::fixed32 val);

double db_identity(double val);


