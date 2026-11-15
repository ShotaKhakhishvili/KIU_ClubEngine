#pragma once

#include <Core/Types/RHITypes.h>

#include <RenderCore/RHI/RHIRenderPassTypes.h>

namespace CE
{

class FRenderTarget
{

public:
    FRenderTarget(uint32 width, uint32 height, bool hasDepth = false);

    FRenderTarget(const FRenderTarget&) = delete;
    FRenderTarget& operator=(const FRenderTarget&) = delete;

    FRenderTarget(FRenderTarget&&) noexcept;
    FRenderTarget& operator=(FRenderTarget&&) noexcept;

    ~FRenderTarget();

    RHI::RenderTargetHandle GetHandle() const;
    void Resize(uint32 width, uint32 height);

    bool HasDepth() const;
    uint32 GetHeight() const;
    uint32 GetWidth() const;

private:
    void Release();
    void Create();

    RHI::RenderTargetHandle target;
    RHI::TextureHandle      colorTexture;
    RHI::TextureHandle      depthTexture;

    uint32                  width;
    uint32                  height;
    bool                    hasDepth;
};

}
