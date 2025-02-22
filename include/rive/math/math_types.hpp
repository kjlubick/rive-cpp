/*
 * Copyright 2022 Rive
 */

#ifndef _RIVE_MATH_TYPES_DEFINED_
#define _RIVE_MATH_TYPES_DEFINED_

#include "rive/rive_types.hpp"
#include <cmath>

namespace rive
{

namespace math
{
constexpr float PI = 3.14159265f;
constexpr float EPSILON = 1.f / (1 << 12); // Common threshold for detecting values near zero.

[[maybe_unused]] inline bool nearly_zero(float a, float tolerance = EPSILON)
{
    assert(tolerance >= 0);
    return fabsf(a) <= tolerance;
}

[[maybe_unused]] inline bool nearly_equal(float a, float b, float tolerance = EPSILON)
{
    return nearly_zero(b - a, tolerance);
}

// Performs a floating point division with conformant IEEE 754 behavior for NaN and Inf.
//
//   Returns +/-Inf if b == 0.
//   Returns 0 if b == +/-Inf.
//   Returns NaN if a and b are both zero.
//   Returns NaN if a and b are both infinite.
//   Returns NaN a or b is NaN.
//
// Reference:
// https://stackoverflow.com/questions/42926763/the-behaviour-of-floating-point-division-by-zero
[[maybe_unused]] static __attribute__((no_sanitize("float-divide-by-zero"), always_inline)) float
ieee_float_divide(float a, float b)
{
    static_assert(std::numeric_limits<float>::is_iec559,
                  "Conformant IEEE 754 behavior for NaN and Inf is required.");
    return a / b;
}

// Reinterprets the underlying bits of src as the given type.
template <typename Dst, typename Src> Dst bit_cast(const Src& src)
{
    static_assert(sizeof(Dst) == sizeof(Src));
    Dst dst;
    __builtin_memcpy(&dst, &src, sizeof(Dst));
    return dst;
}
} // namespace math

template <typename T> T lerp(const T& a, const T& b, float t) { return a + (b - a) * t; }

} // namespace rive

#endif
