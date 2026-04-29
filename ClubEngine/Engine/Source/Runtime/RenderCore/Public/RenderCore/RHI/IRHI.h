#pragma once

#include <Core/Types/RHITypes.h>
#include <Core/Math/Vector.h>

#include <RenderCore/RHI/RHIHandle.h>

#include <cstdint>
#include <string>
#include <memory>

namespace RHI
{
    class IRHI
    {
    public:
        virtual ~IRHI() = default;

        // ------------ Lifetime ------------

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        // ------------ Shader ------------

        virtual ShaderHandle CreateShader(const ShaderDesc& desc) = 0;
        virtual void DestroyShader(ShaderHandle handle) = 0;

        // ------------ Textures ------------

        virtual TextureHandle CreateTexture(
            const TextureDesc& desc,
            const void* data
        ) = 0;

        virtual TextureHandle CreateCubemap(
            const TextureDesc& desc,
            const void* const*  faceData
        ) = 0;

        virtual void DestroyTexture(TextureHandle handle) = 0;
    
        // ------------  Buffers ------------

        virtual BufferHandle CreateBuffer(
            const BufferDesc& desc,
            const void* data
        ) = 0;

        virtual void DestroyBuffer(BufferHandle handle) = 0;

        // ------------ Vertex Array ------------

        virtual VertexArrayHandle CreateVertexArray(
            BufferHandle vertexBufferHandle,
            BufferHandle indexBufferHandle,
            const VertexArrayDesc& vertexArrayDesc
        ) = 0;

        virtual void DestroyVertexArray(VertexArrayHandle handle) = 0;

        // ------------ Render State ------------

        virtual void SetViewport(const ViewportDesc& desc) = 0;
        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void Clear(bool color, bool depth) = 0;

        virtual void SetCullMode(CullMode cullMode) = 0;
        virtual void SetBlendMode(BlendMode blendMode) = 0;
        virtual void SetDepthTest(bool enabled) = 0;
        virtual void SetDepthWrite(bool enabled) = 0;

        // ------------ Binding ------------

        virtual void BindShader(ShaderHandle handle) = 0;
        virtual void BindTexture(TextureHandle handle, uint32_t slot) = 0;
        virtual void BindVertexArray(VertexArrayHandle handle) = 0;

        // ------------ Uniforms ------------

        virtual void SetUniformBool(const std::string& name, bool value) = 0;
        virtual void SetUniformInt(const std::string& name, int32_t value) = 0;
        virtual void SetUniformFloat(const std::string& name, float value) = 0;
        virtual void SetUniformVec2(const std::string& name, const Vec2f& value) = 0;
        virtual void SetUniformVec3(const std::string& name, const Vec3f& value) = 0;
        virtual void SetUniformVec4(const std::string& name, const Vec4f& value) = 0;
        virtual void SetTexture(const std::string& name, TextureHandle texture, uint32_t slot) = 0;

        // ------------ Draw ------------

        virtual void DrawIndexed(const DrawIndexedDesc& desc) = 0;
    };
    
    IRHI& Get();
    void Set(std::unique_ptr<IRHI> newRHI);
}