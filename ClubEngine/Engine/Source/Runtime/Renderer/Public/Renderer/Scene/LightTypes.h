#pragma once

#include <Core/Math/Vector.h>

namespace CE
{

enum class ELightType
{
    Directional,
    Point,
    Spot,
};

struct FLightParameters
{
    FVectorF color{1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;
    float attenuationRadius = 1000.0f;   // local lights only
    float innerConeAngle = 0.0f;         // spot only, degrees
    float outerConeAngle = 44.0f;        // spot only, degrees
};

}
