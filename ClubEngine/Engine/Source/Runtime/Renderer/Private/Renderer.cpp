#include <Core/ClubCore.h>

#include <RenderCore/RHI/IRHI.h>

#include <RHI.OpenGL/GLRHI.h>

#include <Asset/IAssetResolver.h>

#include <Renderer/Renderer.h>

#include "Renderer/FRenderTarget.h"

#include <Renderer/FFrameData.h>
#include <Renderer/Scene/FScene.h>

namespace CE
{

Renderer::Renderer(const RHI::Backend backend)
{
    SetBackend(backend);

    constexpr RHI::BufferDesc frameDataDesc =
    {
        RHI::BufferType::Uniform,
        RHI::BufferUsage::DynamicDraw,
        sizeof(FFrameData)
    };

    frameDataHandle = RHI::Get().CreateBuffer(frameDataDesc, nullptr);
}


Renderer::~Renderer()
{
    RHI::Get().DestroyBuffer(frameDataHandle);
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

void Renderer::Render(const FSceneViewFamily& sceneViewFamily) const
{
    if (!sceneViewFamily.GetScene())
    {
        CE_LOG(Error, "Failed to render with invalid scene");
        return;
    }

    if (!sceneViewFamily.GetRenderTarget()->GetHandle().IsValid())
    {
        CE_LOG(Error, "Failed to render with invalid render target");
        return;
    }

    FFrameData frameData;

    RHI::RenderPassDesc renderPassDesc{};

    renderPassDesc.target = sceneViewFamily.GetRenderTarget()->GetHandle();
    renderPassDesc.colorLoad = sceneViewFamily.ShouldClearTarget() ? RHI::LoadOp::Clear : RHI::LoadOp::Load;
    renderPassDesc.depthLoad = renderPassDesc.colorLoad;

    RHI::Get().BeginRenderPass(renderPassDesc);

    for (const FSceneView& view : sceneViewFamily.GetViews())
    {
        RHI::Get().SetViewport(view.viewRect);
        RHI::Get().SetScissor(view.viewRect);

        frameData.view = view.viewMatrix;
        frameData.proj = view.projMatrix;

        RHI::Get().BindBufferBase(frameDataHandle, frameDataSlot);
        RHI::Get().WriteBuffer(frameDataHandle, &frameData, sizeof(frameData), 0);

        sceneViewFamily.GetScene()->ForEachPrimitive([this](const FPrimitiveSceneProxy& proxy)
        {
            if (!proxy.isVisible || !proxy.staticMesh.IsValid())
                return;

            if (const UStaticMesh* mesh = GetCurrentAssetResolver()->Resolve(proxy.staticMesh))
                DrawStaticMesh(*mesh, proxy.localToWorld);
        });
    }

    RHI::Get().EndRenderPass();
}

void Renderer::Present(const FRenderTarget& renderTarget, const RHI::ViewportDesc& viewDesc)
{
    if (!renderTarget.GetHandle().IsValid())
    {
        CE_LOG(Warning, "Failed to present with invalid render target");
        return;
    }

    const RHI::ViewportDesc source { 0, 0, renderTarget.GetWidth(), renderTarget.GetHeight() };

    RHI::Get().BlitToBackbuffer(renderTarget.GetHandle(), source, viewDesc);
}

}