#include <Engine/Systems/RenderSystem.h>

#include "../CEConvert.h"

RenderSystem::RenderSystem(CE::RenderBackend backend)
{
    renderer = std::make_unique<Renderer>(ToRHI(backend));
}

bool RenderSystem::Initialize()
{
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

    renderer->BeginFrame({400, 400, 800, 800});

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
