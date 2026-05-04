#pragma once

template<typename T>
inline T Lerp     (T a, T b, float t)  { return a + (b - a) * t; }