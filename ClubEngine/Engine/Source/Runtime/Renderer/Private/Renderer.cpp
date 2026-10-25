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

void Renderer::DrawStaticMesh(const UStaticMesh& staticMesh, const FMatrix4x4F& localToWorld) const
{
    const auto resolver = GetCurrentAssetResolver();
    if(!resolver)
    {
        CE_LOG(Error, "Tried To Draw Without An Asset Resolver");
        return;
    }

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

        UMaterial* material = resolver->Resolve(materialHandle);

        if(!material)
        {
            CE_LOG(Warning, "Tried To Draw Sub Mesh With Invalid Null Material");
            continue;
        }

        if(!material->Bind())
        {
            CE_LOG(Warning, "Tried To Draw Sub Mesh But Couldn't Bind Material");
            continue;
        }

        RHI::Get().SetUniformMat4("u_Model", localToWorld);

        const FMaterialRenderState& renderState = material->GetRenderState();

        RHI::Get().SetCullMode(renderState.cullMode);
        RHI::Get().SetBlendMode(renderState.blendMode);
        RHI::Get().SetDepthTest(renderState.depthTest);
        RHI::Get().SetDepthWrite(renderState.depthWrite);

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

    RHI::Get().SetDepthTest(true);
    RHI::Get().SetDepthWrite(true);
    RHI::Get().SetCullMode(RHI::CullMode::Back);
    RHI::Get().SetBlendMode(RHI::BlendMode::Opaque);

    RHI::Get().SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    RHI::Get().Clear(true, true);
}

void Renderer::EndFrame()
{
    // Empty for now lol
}

void Renderer::Render(const FScene& scene)
{
    scene.ForEachPrimitive([this](const FPrimitiveSceneProxy& proxy)
    {
        const UStaticMesh* staticMesh = GetCurrentAssetResolver()->Resolve(proxy.staticMesh);

        if(staticMesh != nullptr)
            DrawStaticMesh(*staticMesh, proxy.localToWorld);
    });

}

}