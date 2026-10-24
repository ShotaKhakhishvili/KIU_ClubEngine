#include <Core/ClubCore.h>

#include <Engine/Systems/RenderSystem.h>
#include <Engine/Systems/WindowSystem.h>
#include <Engine/Systems/WorldSystem.h>


#include "../CEConvert.h"

namespace CE
{

RenderSystem::RenderSystem(WindowSystem* windowSystem,  WorldSystem* worldSystem, ERenderBackend backend)
{
    this->backend = backend;
    this->windowSystem = windowSystem;
    this->worldSystem = worldSystem;
}

bool RenderSystem::Init()
{
    renderer = std::make_unique<Renderer>(ToRHI(backend));
    return renderer != nullptr;
}

void RenderSystem::Shutdown()
{
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

    renderer->Render(worldSystem->GetWorld().GetScene());

    renderer->EndFrame();

    windowSystem->GetWindow()->SwapBuffers();

    return 0;
}

void RenderSystem::SetBackend(CE::ERenderBackend backend)
{
    renderer->SetBackend(ToRHI(backend));
}

}