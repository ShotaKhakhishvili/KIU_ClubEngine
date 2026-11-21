#include <Core/ClubCore.h>

#include <CoreUObject/AssetRegistry.h>

#include <RenderCore/RHI/IRHI.h>

#include <RHI.OpenGL/GLRHI.h>

#include <Asset/UMaterial.h>
#include <Asset/UStaticMesh.h>

#include <Renderer/Renderer.h>
#include "Renderer/FRenderTarget.h"
#include <Renderer/FFrameData.h>
#include <Renderer/Scene/FScene.h>

namespace CE
{

void Renderer::EnsureBufferCapacity(RHI::BufferHandle& handle, uint32& capacity, const uint32 required, const uint32 elementSize)
{
    if (required <= capacity)
        return;

    while (capacity < required)
        capacity *= 2;

    RHI::Get().DestroyBuffer(handle);

    const RHI::BufferDesc desc{
        RHI::BufferType::ShaderStorage,
        RHI::BufferUsage::DynamicDraw,
        capacity * elementSize
    };
    handle = RHI::Get().CreateBuffer(desc, nullptr);
}

void Renderer::UpdateLightSSBOs(const FSceneViewFamily& sceneViewFamily)
{
    directionalLights.clear();
    localLights.clear();

    sceneViewFamily.GetScene()->ForEachLight([&](const FLightSceneProxy& light)
    {
        if (!light.isVisible || light.lightParameters.intensity <= 0)
            return;

        const FVectorF forward = Normalize(light.localToWorld[0].XYZ());
        const FVector4F direction = FVector4F(forward, 0.0f);
        const FVector4F color = light.lightParameters.intensity *
                                FVector4F(light.lightParameters.color, 0.0f);

        if (light.lightType == ELightType::Directional)
        {
            const FGPUDirectionalLight directionalLight{
                .direction = direction,
                .color = color
            };

            directionalLights.push_back(directionalLight);
        }
        else
        {
            const float radius = light.lightParameters.attenuationRadius;

            if (radius <= 0.0f)
                return;

            FVector4F spotAngles(-2.0f, 1.0f, 0.0f, 0.0f);

            if (light.lightType == ELightType::Spot)
            {
                const FLightParameters& params = light.lightParameters;

                const float outerDeg = Clamp(params.outerConeAngle, 1.0f, 89.9f);
                const float innerDeg = Clamp(params.innerConeAngle, 0.0f, outerDeg - 0.01f);

                const float cosOuter = std::cos(ToRadians(outerDeg));
                const float cosInner = std::cos(ToRadians(innerDeg));

                spotAngles.x = cosOuter;
                spotAngles.y = 1.0f / Max(cosInner - cosOuter, KINDA_SMALL_NUMBER<float>);
            }

            const FGPULocalLight localLight{
                .positionInvRadius = FVector4F(light.localToWorld[3].XYZ(), 1.0f / radius),
                .color = color,
                .direction = direction,
                .spotAngles = spotAngles
            };

            localLights.push_back(localLight);
        }
    });

    EnsureBufferCapacity(directionalLightSSBOHandle, directionalLightCountOnBuffer,
        static_cast<uint32>(directionalLights.size()),
        static_cast<uint32>(sizeof(FGPUDirectionalLight)));
    EnsureBufferCapacity(localLightSSBOHandle, localLightCountOnBuffer,
    static_cast<uint32>(localLights.size()),
    static_cast<uint32>(sizeof(FGPULocalLight)));

    RHI::Get().BindBufferBase(directionalLightSSBOHandle, directionalLightSlot);
    if (!directionalLights.empty())
        RHI::Get().WriteBuffer(directionalLightSSBOHandle, directionalLights.data(),
            static_cast<uint32>(directionalLights.size() * sizeof(FGPUDirectionalLight)), 0);

    RHI::Get().BindBufferBase(localLightSSBOHandle, localLightSlot);
    if (!localLights.empty())
        RHI::Get().WriteBuffer(localLightSSBOHandle, localLights.data(),
            static_cast<uint32>(localLights.size() * sizeof(FGPULocalLight)), 0);
}

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

    const RHI::BufferDesc directionalLightSSBODesc{
        RHI::BufferType::ShaderStorage,
        RHI::BufferUsage::DynamicDraw,
         static_cast<uint32>(sizeof(FGPUDirectionalLight)) * directionalLightCountOnBuffer,
    };
    directionalLightSSBOHandle = RHI::Get().CreateBuffer(directionalLightSSBODesc, nullptr);

    const RHI::BufferDesc localLightSSBODesc{
        RHI::BufferType::ShaderStorage,
        RHI::BufferUsage::DynamicDraw,
        static_cast<uint32>(sizeof(FGPULocalLight)) * localLightCountOnBuffer
    };
    localLightSSBOHandle = RHI::Get().CreateBuffer(localLightSSBODesc, nullptr);
}


Renderer::~Renderer()
{
    RHI::Get().DestroyBuffer(frameDataHandle);
    RHI::Get().DestroyBuffer(directionalLightSSBOHandle);
    RHI::Get().DestroyBuffer(localLightSSBOHandle);
    RHI::Get().Shutdown();
}

void Renderer::DrawStaticMesh(const UStaticMesh& staticMesh, const FMatrix4x4F& localToWorld) const
{
    if(!staticMesh.GetVertexArray().IsValid())
    {
        CE_LOG(Warning, "Tried To Draw Sub Mesh With Invalid Vertex Array");
        return;
    }

    const auto& subMeshes = staticMesh.GetSubMeshes();
    const auto& materials = staticMesh.GetMaterials();

    RHI::Get().BindVertexArray(staticMesh.GetVertexArray());

    for(const auto& subMesh : subMeshes)
    {

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

        UMaterial* material = Resolve(materialHandle);

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

        const RHI::FRenderStateDesc& renderState = material->GetRenderState();

        RHI::Get().SetCullMode(renderState.cullMode);
        RHI::Get().SetBlendMode(renderState.blendMode);
        RHI::Get().SetDepthTest(renderState.depthTest);
        RHI::Get().SetDepthWrite(renderState.depthWrite);

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

void Renderer::Render(const FSceneViewFamily& sceneViewFamily)
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

    UpdateLightSSBOs(sceneViewFamily);

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

        frameData.viewOrigin = FVector4F{
            static_cast<float>(view.viewOrigin.x),
            static_cast<float>(view.viewOrigin.y),
            static_cast<float>(view.viewOrigin.z), 0.0f
        };

        frameData.lightCounts = FVector4I{
            static_cast<int>(directionalLights.size()),
            static_cast<int>(localLights.size()), 0, 0
        };

        RHI::Get().BindBufferBase(frameDataHandle, frameDataSlot);
        RHI::Get().WriteBuffer(frameDataHandle, &frameData, sizeof(frameData), 0);

        sceneViewFamily.GetScene()->ForEachPrimitive([this](const FPrimitiveSceneProxy& proxy)
        {
            if (!proxy.isVisible || !proxy.staticMesh.IsValid())
                return;

            if (const UStaticMesh* mesh = Resolve(proxy.staticMesh))
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