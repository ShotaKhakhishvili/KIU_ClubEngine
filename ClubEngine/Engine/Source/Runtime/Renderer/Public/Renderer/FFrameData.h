#pragma once
#include "Core/Math/Matrix4x4.h"

namespace CE
{

struct FFrameData
{
    FMatrix4x4F view;
    FMatrix4x4F proj;
};

static_assert(sizeof(FFrameData) == 128, "Must match FrameData.glsl (std140)");

}