#include <RenderCore/RHI/IRHI.h>

#include <Renderer/FRenderTarget.h>

namespace CE
{

FRenderTarget::FRenderTarget(uint32 width, uint32 height, bool hasDepth)
    : width(width), height(height), hasDepth(hasDepth)
{
    Create();
}

FRenderTarget::FRenderTarget(FRenderTarget&& other) noexcept
    : target(other.target), colorTexture(other.colorTexture), depthTexture(other.depthTexture),
        width(other.width), height(other.height), hasDepth(other.hasDepth)
{
    other.colorTexture.Invalidate();
    other.depthTexture.Invalidate();
    other.target.Invalidate();
}

FRenderTarget& FRenderTarget::operator=(FRenderTarget&& other) noexcept
{
    if (this == &other)
        return *this;

    Release();

    width = other.width;
    height = other.height;
    hasDepth = other.hasDepth;
    colorTexture = other.colorTexture;
    depthTexture = other.depthTexture;
    target = other.target;

    other.colorTexture.Invalidate();
    other.depthTexture.Invalidate();
    other.target.Invalidate();

    return *this;
}
FRenderTarget::~FRenderTarget()
{
    Release();
}
RHI::RenderTargetHandle FRenderTarget::GetHandle() const
{
    return target;
}

void FRenderTarget::Resize(const uint32 newWidth, const uint32 newHeight)
{
    if (newWidth == width && newHeight == height)
        return;                                 // nothing changed

    if (newWidth == 0 || newHeight == 0)
        return;

    Release();

    width = newWidth;
    height = newHeight;

    Create();
}

bool FRenderTarget::HasDepth()                      const { return hasDepth;        }
uint32 FRenderTarget::GetHeight()                   const { return height;          }
uint32 FRenderTarget::GetWidth()                    const { return width;           }
RHI::TextureHandle FRenderTarget::GetColorTexture() const { return colorTexture;    }
RHI::TextureHandle FRenderTarget::GetDepthTexture() const { return depthTexture;    }

void FRenderTarget::Release()
{
    if (target.IsValid())
    {
        RHI::Get().DestroyRenderTarget(target);
        target.Invalidate();
    }
    if (colorTexture.IsValid())
    {
        RHI::Get().DestroyTexture(colorTexture);
        colorTexture.Invalidate();
    }
    if (depthTexture.IsValid())
    {
        RHI::Get().DestroyTexture(depthTexture);
        depthTexture.Invalidate();
    }
}

void FRenderTarget::Create()
{
    RHI::TextureDesc desc = {.width = width, .height = height, .generateMipmaps = false};

    colorTexture =  RHI::Get().CreateTexture(desc, nullptr);

    if (hasDepth)
    {
        desc.readFormat = RHI::TextureFormat::Depth;
        desc.format = RHI::TextureInternalFormat::DepthComponent;
        desc.pixelType = RHI::TexturePixelType::Float;

        depthTexture = RHI::Get().CreateTexture(desc, nullptr);
    }

    target = RHI::Get().CreateRenderTarget({colorTexture, depthTexture});
}

}