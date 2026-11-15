#pragma once

#include <Core/Types/CoreTypes.h>
#include <Core/Types/RHITypes.h>

namespace CE::RHI
{

class GLTexture;

class GLRenderTarget
{
public:
    GLRenderTarget(const GLTexture& colorTexture, const GLTexture* depthTexture);
    ~GLRenderTarget();

    GLRenderTarget(const GLRenderTarget&) = delete;
    GLRenderTarget& operator=(const GLRenderTarget&) = delete;

    GLRenderTarget(GLRenderTarget&& other) noexcept;
    GLRenderTarget& operator=(GLRenderTarget&& other) noexcept;

    void Bind() const;

    bool IsComplete() const noexcept;

    uint32 GetID() const noexcept;
    uint32 GetWidth() const noexcept;
    uint32 GetHeight() const noexcept;

private:
    void Delete();

    RenderTargetID ID{0};
    uint32 width{0};
    uint32 height{0};
    bool complete{false};
};

}