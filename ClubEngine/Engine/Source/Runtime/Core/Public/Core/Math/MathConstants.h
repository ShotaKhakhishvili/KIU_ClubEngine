#pragma once

#include <concepts>

namespace CE
{

template<std::floating_point T>
inline constexpr T PI         = T(3.14159265358979323846);

template<std::floating_point T>
inline constexpr T TWO_PI     = T(6.28318530717958647692);

template<std::floating_point T>
inline constexpr T HALF_PI    = T(1.57079632679489661923);

template<std::floating_point T>
inline constexpr T INV_PI     = T(0.31830988618379067154);

template<std::floating_point T>
inline constexpr T DEG_TO_RAD = PI<T> / T(180);

template<std::floating_point T>
inline constexpr T RAD_TO_DEG = T(180) / PI<T>;

template<std::floating_point T>
inline constexpr T SMALL_NUMBER = T(1e-8);

template<std::floating_point T>
inline constexpr T KINDA_SMALL_NUMBER = T(1e-4);

inline constexpr float  PI_F          = PI<float>;
inline constexpr float  TWO_PI_F      = TWO_PI<float>;
inline constexpr float  HALF_PI_F     = HALF_PI<float>;
inline constexpr float  DEG_TO_RAD_F  = DEG_TO_RAD<float>;
inline constexpr float  RAD_TO_DEG_F  = RAD_TO_DEG<float>;    

}