#pragma once

#include <CoreUObject/TObjectHandle.h>

#include <Engine/Components/UCameraComponent.h>

namespace CE
{

struct FViewTarget
{
    TObjectHandle<UCameraComponent> camera;
    float x = 0.0f;
    float y = 0.0f;
    float width = 1.0f;
    float height = 1.0f;
};

}