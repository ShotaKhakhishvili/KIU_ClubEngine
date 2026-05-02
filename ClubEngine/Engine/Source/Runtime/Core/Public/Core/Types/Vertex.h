#pragma once

#include <Core/Math/Vector.h>

namespace CE
{
    struct Vertex
    {
        FVectorF coord{};
        FVectorF normal{};
        FVectorF color{};
        FVector2F texUV{};
    };
}