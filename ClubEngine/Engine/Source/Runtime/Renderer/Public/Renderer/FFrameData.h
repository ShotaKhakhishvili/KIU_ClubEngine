#pragma once

#include <Core/Math/Matrix4x4.h>

namespace CE
{

struct FFrameData
{
    FMatrix4x4F view;
    FMatrix4x4F proj;
    FVector4F viewOrigin;
    FVector4I lightCounts;
};

static_assert(sizeof(FFrameData) == 160, "Must match FrameData.glsl (std140)");

}