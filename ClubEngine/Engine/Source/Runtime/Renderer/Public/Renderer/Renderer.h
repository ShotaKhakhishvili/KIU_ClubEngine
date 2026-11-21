#pragma once

#include <Core/Types/RHITypes.h>

#include <Asset/UStaticMesh.h>

#include <Renderer/Scene/LightTypes.h>
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

    void Render(const FSceneViewFamily& sceneViewFamily);
    static void Present(const FRenderTarget& renderTarget, const RHI::ViewportDesc& viewDesc);

    void SetBackend(RHI::Backend backend);

    void DrawStaticMesh(const UStaticMesh& staticMesh, const TArray<TObjectHandle<UMaterial>>& overrideMaterials, const FMatrix4x4F& localToWorld) const;

private:
    static void EnsureBufferCapacity(RHI::BufferHandle& handle, uint32& capacity, uint32 required, uint32 elementSize);
    void UpdateLightSSBOs(const FSceneViewFamily& sceneViewFamily);

    RHI::Backend backend;

    // Frame Data
    RHI::BufferHandle frameDataHandle;
    uint32 frameDataSlot = 0;

    RHI::BufferHandle directionalLightSSBOHandle;
    uint32 directionalLightSlot = 1;
    TArray<FGPUDirectionalLight> directionalLights;
    uint32 directionalLightCountOnBuffer = 16;

    RHI::BufferHandle localLightSSBOHandle;
    uint32 localLightSlot = 2;
    TArray<FGPULocalLight> localLights;
    uint32 localLightCountOnBuffer = 16;
};

}