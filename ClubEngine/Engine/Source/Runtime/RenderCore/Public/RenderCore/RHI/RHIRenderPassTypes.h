#pragma once

#include <Core/Types/RHITypes.h>
#include <Core/Math/Vector.h>

#include <RenderCore/RHI/RHIHandle.h>

namespace CE::RHI
{

struct RenderTargetDesc
{
    TextureHandle colorAttachment;
    TextureHandle depthAttachment;      // invalid means no depth buffer
};

struct RenderPassDesc
{
    RenderTargetHandle target;          // invalid means it is a backbuffer

    LoadOp colorLoad = LoadOp::Clear;
    LoadOp depthLoad = LoadOp::Clear;

    FVector4F clearColor{ 0.1f, 0.1f, 0.1f, 1.0f };
    float clearDepth = 1.0f;
};

}