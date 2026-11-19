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

struct FGPUDirectionalLight
{
    FVector4F direction;            // xyz = direction light travels, w unused
    FVector4F color;                // rgb = color * intensity, w unused
};

struct FGPULocalLight
{
    FVector4F positionInvRadius;    // xyz = world position, w = 1 / attenuation radius
    FVector4F color;                // rgb = color * intensity, w unused
    FVector4F direction;            // xyz = spot direction (unused for point), w unused
    FVector4F spotAngles;           // x = cos(outer), y = 1 / (cos(inner) - cos(outer)), zw unused
};

static_assert(sizeof(FGPUDirectionalLight) == 32, "Must match Lights.glsl (std430)");
static_assert(sizeof(FGPULocalLight) == 64, "Must match Lights.glsl (std430)");

}
