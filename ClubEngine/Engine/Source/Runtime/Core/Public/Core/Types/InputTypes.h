#pragma once
#include <cstdint>

namespace CE
{
    enum class EKeyCode : uint16_t
    {
        Space = 32,
        A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71,
        H = 72, I = 73, J = 74, K = 75, L = 76, M = 77, N = 78,
        O = 79, P = 80, Q = 81, R = 82, S = 83, T = 84, U = 85,
        V = 86, W = 87, X = 88, Y = 89, Z = 90,
        Escape = 256, Enter = 257, Tab = 258,
        Up = 265, Down = 264, Left = 263, Right = 262
    };

    enum class EMouseButton : uint8_t
    {
        Left = 0, Right = 1, Middle = 2
    };

    enum class EInputAction : uint8_t
    {
        Release = 0, Press = 1, Repeat = 2
    };
}