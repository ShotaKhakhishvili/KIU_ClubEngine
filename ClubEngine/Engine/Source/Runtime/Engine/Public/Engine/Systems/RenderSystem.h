#pragma once

#include <Asset/UStaticMesh.h>

#include <Renderer/Renderer.h>

#include <Engine/EngineTypes.h>
#include <Engine/ISystem.h>

namespace CE
{

class WindowSystem;
class WorldSystem;

class RenderSystem final : public ISystem
{
public:

    RenderSystem(WindowSystem* windowSystem, WorldSystem* worldSystem, CE::RenderBackend backend = CE::RenderBackend::OpenGL); // Make OpenGL a default (not like we don't have anything else)

    const char* GetName() const noexcept    override;

    bool    Init        ()                      override;
    void    Shutdown    ()                      override;
    int     Update      (float dt)              override;

    void SetBackend(CE::RenderBackend backend);
private:
    std::unique_ptr<Renderer>   renderer;
    WindowSystem*               windowSystem;
    WorldSystem*                worldSystem;
    RenderBackend               backend;
};

}