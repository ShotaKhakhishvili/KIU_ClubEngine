#include <Core/ClubCore.h>

#include <Engine/Systems/RenderSystem.h>

#include "../CEConvert.h"

namespace CE
{

RenderSystem::RenderSystem(WindowSystem* windowSystem, CE::RenderBackend backend)
{
    this->backend = backend;
    this->windowSystem = windowSystem;
}

bool RenderSystem::Initialize()
{
    renderer = std::make_unique<Renderer>(ToRHI(backend));
    return renderer != nullptr;
}

void RenderSystem::Shutdown()
{
    meshesToRender.clear();
    renderer.reset();
}

const char* RenderSystem::GetName() const noexcept
{
    return "Render System";
}

int RenderSystem::Update(float dt)
{
    (void)dt;

    renderer->BeginFrame({0, 0, windowSystem->GetWindow()->GetWidth(), windowSystem->GetWindow()->GetHeight()});

    for(auto staticMesh : meshesToRender)
    {
        renderer->DrawStaticMesh(*staticMesh);
    }

    renderer->EndFrame();

    return 0;
}

void RenderSystem::SetBackend(CE::RenderBackend backend)
{
    renderer->SetBackend(ToRHI(backend));
}

void RenderSystem::AddMesh(UStaticMesh* newMesh)
{
    meshesToRender.push_back(newMesh);
}

}