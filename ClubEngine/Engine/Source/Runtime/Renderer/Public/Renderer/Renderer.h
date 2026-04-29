#pragma once

#include <Core/Types/RHITypes.h>

#include <Asset/UStaticMesh.h>

class Renderer
{
public:
    Renderer(RHI::Backend backend);
    ~Renderer();

    // These are temporary for now. Later we will have a UWorld, which the renderer will receive and render fully.

    void BeginFrame(const RHI::ViewportDesc& viewportDesc);
    void EndFrame();

    void SetBackend(RHI::Backend backend);

    void DrawStaticMesh(const UStaticMesh& staticMesh) const;
private:
    RHI::Backend backend;
};