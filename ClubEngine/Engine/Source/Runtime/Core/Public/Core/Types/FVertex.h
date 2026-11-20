#pragma once

#include <Core/Math/Vector.h>

namespace CE
{

struct FVertex
{
    FVectorF coord{};
    FVectorF normal{};
    FVector4F color{1.0f, 1.0f, 1.0f, 1.0f};
    FVector2F texUV{};
    FVector4F tangent{1.0f, 0.0f, 0.0f, 1.0f};
};

}