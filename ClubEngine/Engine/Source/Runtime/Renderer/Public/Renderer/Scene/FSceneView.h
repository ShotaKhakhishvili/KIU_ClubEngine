#pragma once

#include <Core/Math/Matrix4x4.h>
#include <Core/Math/Vector.h>
#include <Core/Types/RHITypes.h>

namespace CE
{

struct FSceneView
{
    FMatrix4x4F viewMatrix;
    FMatrix4x4F projMatrix;

    FVector viewOrigin;
    RHI::ViewportDesc viewRect;
};

}