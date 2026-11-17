#pragma once

#include <Asset/UStaticMesh.h>

#include <Renderer/Renderer.h>
#include <Renderer/FRenderTarget.h>

#include <Engine/EngineTypes.h>
#include <Engine/ISystem.h>

namespace CE
{

class WindowSystem;
class WorldSystem;
class UWorld;

class RenderSystem final : public ISystem
{
public:

    RenderSystem(WindowSystem* windowSystem, WorldSystem* worldSystem, CE::ERenderBackend backend = CE::ERenderBackend::OpenGL); // Make OpenGL a default (not like we don't have anything else)

    const char* GetName() const noexcept    override;

    bool    Init        ()                      override;
    void    Shutdown    ()                      override;
    int     Update      (float dt)              override;

    void SetBackend(CE::ERenderBackend backend);

    void SetFixedTargetSize(uint32 width, uint32 height);
    bool ReadSceneColor(TArray<uint8>& outPixels) const;

private:
    TArray<FSceneView> BuildSceneViews(const UWorld& world) const;

    std::unique_ptr<FRenderTarget> sceneTarget;

    std::unique_ptr<Renderer>   renderer;
    WindowSystem*               windowSystem;
    WorldSystem*                worldSystem;
    ERenderBackend              backend;

    uint32 fixedWidth  = 0;
    uint32 fixedHeight = 0;
};

}