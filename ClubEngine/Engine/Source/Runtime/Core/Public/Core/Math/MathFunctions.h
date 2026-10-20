// ─── AI-Assisted Code ────────────────────────────────────────────────────────
//
//  This file was generated with the assistance of Claude (Anthropic).
//  Reviewed and approved by: Shota Khakhishvili
//  Date: 2026-05-08
//
//  If you modify this file, remove or update this notice accordingly.
//
// ─────────────────────────────────────────────────────────────────────────────



#pragma once

#include <Core/Math/MathConstants.h>

#include <cmath>
#include <concepts>
#include <type_traits>

namespace CE
{

template<typename T>
constexpr T Lerp     (T a, T b, float t)  { return a + (b - a) * t; }

template<std::floating_point T>
constexpr T InverseLerp(T a, T b, T value) { return (value - a) / (b - a); }

template<typename T>
constexpr T Clamp(T value, T lo, T hi)
{
    return value < lo ? lo : (value > hi ? hi : value);
}

template<typename T>
constexpr T Saturate(T value)
{
    return Clamp(value, T(0), T(1));
}

template<typename T>
constexpr T Min(T a, T b) { return a < b ? a : b; }

template<typename T>
constexpr T Max(T a, T b) { return a > b ? a : b; }

template<typename T>
constexpr T Abs(T v) { return v < T(0) ? -v : v; }

template<typename T>
constexpr T Sign(T v) { return v < T(0) ? T(-1) : (v > T(0) ? T(1) : T(0)); }

template<typename T>
constexpr T Square(T v) { return v * v; }

template<typename T>
constexpr T Cube(T v) { return v * v * v; }

template<std::floating_point T>
constexpr bool NearlyZero(T value, T tolerance = SMALL_NUMBER<T>)
{
    return Abs(value) < tolerance;
}

template<std::floating_point T>
constexpr bool NearlyEqual(T a, T b, T tolerance = SMALL_NUMBER<T>)
{
    return Abs(a - b) < tolerance;
}

template<std::floating_point T>
constexpr T ToRadians(T degrees) { return degrees * DEG_TO_RAD<T>; }

template<std::floating_point T>
constexpr T ToDegrees(T radians) { return radians * RAD_TO_DEG<T>; }

template<std::floating_point T>
inline T RSqrt(T v)
{
    return v > SMALL_NUMBER<T> ? T(1) / std::sqrt(v) : T(0);
}

}