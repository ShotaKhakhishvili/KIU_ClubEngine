#pragma once

#include <Core/Types/RHITypes.h>

#include <Asset/UStaticMesh.h>

#include <Renderer/Scene/FSceneViewFamily.h>
#include <Renderer/FFrameData.h>

namespace CE
{

class FScene;

class Renderer
{
public:
    explicit Renderer(RHI::Backend backend);
    ~Renderer();

    void Render(const FSceneViewFamily& sceneViewFamily) const;
    static void Present(const FRenderTarget& renderTarget, const RHI::ViewportDesc& viewDesc);

    void SetBackend(RHI::Backend backend);

    void DrawStaticMesh(const UStaticMesh& staticMesh, const FMatrix4x4F& localToWorld) const;

private:

    RHI::Backend backend;

    // Frame Data
    RHI::BufferHandle frameDataHandle;
    uint32 frameDataSlot = 0;
};

}