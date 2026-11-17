#include <Core/ClubCore.h>
#include <Core/Math/MathFunctions.h>

#include <RenderCore/RHI/IRHI.h>

#include <Engine/World/UWorld.h>
#include <Engine/World/UCameraComponent.h>
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

    const IWindow* window = windowSystem->GetWindow();
    sceneTarget = std::make_unique<FRenderTarget>(window->GetWidth(), window->GetHeight(), true);

    return renderer != nullptr;
}

void RenderSystem::Shutdown()
{
    sceneTarget.reset();
    renderer.reset();
}

const char* RenderSystem::GetName() const noexcept
{
    return "Render System";
}

int RenderSystem::Update(float dt)
{
    (void)dt;

    const IWindow* window = windowSystem->GetWindow();
    const uint32 windowWidth = window->GetWidth();
    const uint32 windowHeight = window->GetHeight();

    if (windowWidth == 0 || windowHeight == 0)
        return 0;

    RHI::Get().SetBackbufferSize(windowWidth, windowHeight);

    const bool fixedSize = fixedWidth > 0 && fixedHeight > 0;
    sceneTarget->Resize(fixedSize ? fixedWidth  : windowWidth,
                        fixedSize ? fixedHeight : windowHeight);

    const FSceneViewFamily sceneViewFamily(&worldSystem->GetWorld().GetScene(), sceneTarget.get(),
        BuildSceneViews(worldSystem->GetWorld()));

    renderer->Render(sceneViewFamily);
    Renderer::Present(*sceneTarget, { 0, 0, windowWidth, windowHeight });

    windowSystem->GetWindow()->SwapBuffers();

    return 0;
}

void RenderSystem::SetBackend(const CE::ERenderBackend backend)
{
    renderer->SetBackend(ToRHI(backend));
}

void RenderSystem::SetFixedTargetSize(uint32 width, uint32 height)
{
    fixedWidth  = width;
    fixedHeight = height;
}

bool RenderSystem::ReadSceneColor(TArray<uint8>& outPixels) const
{
    const uint32 w = sceneTarget->GetWidth();
    const uint32 h = sceneTarget->GetHeight();
    outPixels.resize(static_cast<size_t>(w) * h * 4);

    return RHI::Get().ReadTexture(sceneTarget->GetColorTexture(), { 0, 0, w, h },
                              outPixels.data(), static_cast<uint32>(outPixels.size()));
}

TArray<FSceneView> RenderSystem::BuildSceneViews(const UWorld& world) const
{
    TArray<FSceneView> views;

    const float targetWidth = static_cast<float>(sceneTarget->GetWidth());
    const float targetHeight = static_cast<float>(sceneTarget->GetHeight());

    for (const FViewTarget& viewTarget : world.GetViewTargets())
    {
        const UCameraComponent* camera = world.ResolveComponent(viewTarget.camera);
        if (camera == nullptr || !camera->IsRegistered())
            continue;

        FSceneView view;
        view.viewRect = {
            static_cast<uint32>(viewTarget.x * targetWidth),
            static_cast<uint32>(viewTarget.y * targetHeight),
            static_cast<uint32>(viewTarget.width * targetWidth),
            static_cast<uint32>(viewTarget.height * targetHeight)
        };

        if (view.viewRect.width == 0 || view.viewRect.height == 0)
            continue;

        const auto [pos, rot, fov, zNear, zFar] = camera->GetCameraView();

        const float aspect = static_cast<float>(view.viewRect.width) / static_cast<float>(view.viewRect.height);

        const FVectorF eye(pos);
        const FVectorF forward(rot.GetForward());
        const FVectorF up(rot.GetUp());

        view.viewOrigin = pos;
        view.viewMatrix = FMatrix4x4F::LookAt(eye, eye + forward, up);
        view.projMatrix = FMatrix4x4F::Perspective(ToRadians(fov), aspect, zNear, zFar);

        views.push_back(view);
    }

    return views;
}

}