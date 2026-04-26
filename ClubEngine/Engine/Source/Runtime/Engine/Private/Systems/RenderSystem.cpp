#include <Engine/Systems/RenderSystem.h>

RenderSystem::RenderSystem(RHI::Backend backend)
{
    renderer = std::make_unique<Renderer>(backend);
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

void RenderSystem::SetBackend(RHI::Backend backend)
{
    renderer->SetBackend(backend);
}

void RenderSystem::AddMesh(UStaticMesh* newMesh)
{
    meshesToRender.push_back(newMesh);
}
