#pragma once

#include <RenderCore/RHI/RHIHandle.h>
#include <RenderCore/RHI/RHITypes.h>
#include <RenderCore/Vector.h>

#include <cstdint>
#include <string>

class IRHI
{
public:
    virtual ~IRHI() = default;

    // ------------ Lifetime ------------

    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    // ------------ Shader ------------

    virtual RHIShaderHandle CreateShader(const RHIShaderDesc& desc) = 0;
    virtual void DestroyShader(RHIShaderHandle handle) = 0;

    // ------------ Textures ------------

    virtual RHITextureHandle CreateTexture(
        const RHITextureDesc& desc,
        const void* data
    ) = 0;

    virtual RHITextureHandle CreateCubemap(
        const RHITextureDesc& desc,
        const void* const*  faceData
    ) = 0;

    virtual void DestroyTexture(RHITextureHandle handle) = 0;
  
    // ------------  Buffers ------------

    virtual RHIBufferHandle CreateBuffer(
        const RHIBufferDesc& desc,
        const void* data
    ) = 0;

    virtual void DestroyBuffer(RHIBufferHandle handle) = 0;

    // ------------ Vertex Array ------------

    virtual RHIVertexArrayHandle CreateVertexArray(
        RHIBufferHandle vertexBufferHandle,
        RHIBufferHandle indexBufferHandle,
        const RHIVertexArrayDesc& vertexArrayDesc
    ) = 0;

    virtual void DestroyVertexArray(RHIVertexArrayHandle handle) = 0;

    // ------------ Render State ------------

    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(float r, float g, float b, float a) = 0;
    virtual void Clear(bool color, bool depth) = 0;

    virtual void SetCullMode(CullMode cullMode) = 0;
    virtual void SetBlendMode(BlendMode blendMode) = 0;
    virtual void SetDepthTest(bool enabled) = 0;
    virtual void SetDepthWrite(bool enabled) = 0;

    // ------------ Binding ------------

    virtual void BindShader(RHIShaderHandle handle) = 0;
    virtual void BindTexture(RHITextureHandle handle, uint32_t slot) = 0;
    virtual void BindVertexArray(RHIVertexArrayHandle handle) = 0;

    // ------------ Uniforms ------------

    virtual void SetUniformBool(const std::string& name, bool value) = 0;
    virtual void SetUniformInt(const std::string& name, int32_t value) = 0;
    virtual void SetUniformFloat(const std::string& name, float value) = 0;
    virtual void SetUniformVec2(const std::string& name, const Vec2f& value) = 0;
    virtual void SetUniformVec3(const std::string& name, const Vec3f& value) = 0;
    virtual void SetUniformVec4(const std::string& name, const Vec4f& value) = 0;

    // ------------ Draw ------------

    virtual void DrawIndexed(const RHIDrawIndexedDesc& desc) = 0;
};