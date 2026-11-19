#pragma once

#include <Core/Math/Matrix4x4.h>

#include <Renderer/Scene/LightTypes.h>

namespace CE
{

struct FLightSceneProxy
{
    ELightType lightType = ELightType::Point;
    FLightParameters lightParameters = FLightParameters{};
    FMatrix4x4F localToWorld = FMatrix4x4F::Identity();
    bool isVisible = true;
};

}