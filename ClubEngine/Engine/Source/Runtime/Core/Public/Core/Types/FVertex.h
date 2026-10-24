#pragma once

#include <Core/Math/Vector.h>

namespace CE
{
    struct FVertex
    {
        FVectorF coord{};
        FVectorF normal{};
        FVectorF color{};
        FVector2F texUV{};
    };
}