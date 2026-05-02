#include <Core/ClubCore.h>

#include <RenderCore/RHI/IRHI.h>

#include <RHI.OpenGL/GLRHI.h>

#include <Asset/IAssetResolver.h>

#include <Renderer/Renderer.h>

namespace CE
{

Renderer::Renderer(RHI::Backend backend)
{
    SetBackend(backend);
}

Renderer::~Renderer()
{
    RHI::Get().Shutdown();
}

void Renderer::DrawStaticMesh(const UStaticMesh& staticMesh) const
{
    const auto& subMeshes = staticMesh.GetSubMeshes();
    const auto& materials = staticMesh.GetMaterials();

    for(const auto& subMesh : subMeshes)
    {
        if(!subMesh.vertexArray.IsValid())
        {
            CE_LOG(Warning, "Tried To Draw Sub Mesh With Invalid Vertex Array");
            continue;
        }

        if(subMesh.materialIndex >= materials.size())
        {
            CE_LOG(Warning, "Tried To Draw Sub Mesh With Invalid Material index");
            continue;
        }

        const auto materialHandle = materials[subMesh.materialIndex];

        if(!materialHandle.IsValid())
        {
            CE_LOG(Warning, "Tried To Draw Sub Mesh With Invalid Material");
            continue;
        }

        const auto resolver = GetCurrentAssetResolver();

        UMaterial* material = resolver->Resolve(materialHandle);
        material->Bind();

        RHI::Get().BindVertexArray(subMesh.vertexArray);
        RHI::Get().DrawIndexed(subMesh.drawDesc);
    }
}

void Renderer::SetBackend(RHI::Backend backend)
{
    this->backend = backend;
    
    switch(backend)
    {
        case RHI::Backend::OpenGL:
        {
            std::unique_ptr<RHI::GLRHI> glrhi = std::make_unique<RHI::GLRHI>();
            glrhi->Init();
            RHI::Set(std::move(glrhi));
            return;
        }
    }
}

void Renderer::BeginFrame(const RHI::ViewportDesc& viewportDesc)
{
    RHI::Get().SetViewport(viewportDesc);
    RHI::Get().SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    RHI::Get().Clear(true, true);

    RHI::Get().SetDepthTest(true);
    RHI::Get().SetDepthWrite(true);
    RHI::Get().SetCullMode(RHI::CullMode::Back);
    RHI::Get().SetBlendMode(RHI::BlendMode::Opaque);
}

void Renderer::EndFrame()
{
    // Empty for now lol
}

}