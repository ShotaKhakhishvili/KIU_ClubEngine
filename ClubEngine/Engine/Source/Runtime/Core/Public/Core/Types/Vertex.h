#pragma once

#include <Core/Math/Vector.h>

struct Vertex
{
    Vec3f coord{};
    Vec3f normal{};
    Vec3f color{};
    Vec2f texUV{};
};