#pragma once

#include <Core/Types/RHITypes.h>

#include <Asset/UStaticMesh.h>

#include <Renderer/Scene/FScene.h>

namespace CE
{

class FScene;

class Renderer
{
public:
    Renderer(RHI::Backend backend);
    ~Renderer();

    void BeginFrame(const RHI::ViewportDesc& viewportDesc);
    void Render(const FScene& scene);
    void EndFrame();

    void SetBackend(RHI::Backend backend);

    void DrawStaticMesh(const UStaticMesh& staticMesh, const FMatrix4x4F& localToWorld) const;
private:
    RHI::Backend backend;
};

}