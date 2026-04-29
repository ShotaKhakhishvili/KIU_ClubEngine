#pragma once

#include <Core/Types/RHITypes.h>

#include <Asset/UStaticMesh.h>

#include <Renderer/Renderer.h>

#include <Engine/EngineTypes.h>
#include <Engine/ISystem.h>

class RenderSystem final : public ISystem
{
public:

    RenderSystem(CE::RenderBackend backend = CE::RenderBackend::OpenGL); // Make OpenGL a default (not like we don't have anything else)

    const char* GetName() const noexcept override;

    bool Initialize() override;
    void Shutdown() override;
    int Update(float dt) override;

    void SetBackend(RHI::Backend backend);

    void AddMesh(UStaticMesh* newMesh);

private:
    std::vector<UStaticMesh*> meshesToRender;   // testing purposes
    std::unique_ptr<Renderer> renderer;
};