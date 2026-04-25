#pragma once

#include <memory>
#include <unordered_map>

#include <RenderCore/RHI/IRHI.h>

class Shader;
class Texture;
class VAO;
class VBO;
class EBO;

class OpenGLRHI final : public IRHI
{
public:
    void Init() override;
    void Shutdown() override;

    RHIShaderHandle CreateShader(const RHIShaderDesc& desc) override;
    void DestroyShader(RHIShaderHandle shader) override;

    RHITextureHandle CreateTexture(const RHITextureDesc& desc, const void* data) override;
    RHITextureHandle CreateCubemap(const RHITextureDesc& desc, const void* const* faceData) override;
    void DestroyTexture(RHITextureHandle texture) override;

    RHIBufferHandle CreateBuffer(const RHIBufferDesc& desc, const void* data) override;
    void DestroyBuffer(RHIBufferHandle buffer) override;

    RHIVertexArrayHandle CreateVertexArray(
        RHIBufferHandle vertexBuffer,
        RHIBufferHandle indexBuffer,
        const RHIVertexArrayDesc& desc
    ) override;

    void DestroyVertexArray(RHIVertexArrayHandle vertexArray) override;

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void SetClearColor(float r, float g, float b, float a) override;
    void Clear(bool color, bool depth) override;

    void SetCullMode(CullMode mode) override;
    void SetBlendMode(BlendMode mode) override;
    void SetDepthTest(bool enabled) override;
    void SetDepthWrite(bool enabled) override;

    void BindShader(RHIShaderHandle shader) override;
    void BindVertexArray(RHIVertexArrayHandle vertexArray) override;
    void BindTexture(RHITextureHandle texture, uint32_t slot) override;

    void SetUniformBool(const std::string& name, bool value) override;
    void SetUniformInt(const std::string& name, int32_t value) override;
    void SetUniformFloat(const std::string& name, float value) override;
    void SetUniformVec2(const std::string& name, const Vec2f& value) override;
    void SetUniformVec3(const std::string& name, const Vec3f& value) override;
    void SetUniformVec4(const std::string& name, const Vec4f& value) override;

    void DrawIndexed(const RHIDrawIndexedDesc& desc) override;

private:
    uint32_t nextShaderHandle = 1;
    uint32_t nextTextureHandle = 1;
    uint32_t nextBufferHandle = 1;
    uint32_t nextVertexArrayHandle = 1;

    RHIShaderHandle currentShader{};

    std::unordered_map<uint32_t, std::unique_ptr<Shader>> shaders;
    std::unordered_map<uint32_t, std::unique_ptr<Texture>> textures;
    std::unordered_map<uint32_t, std::unique_ptr<VBO>> vertexBuffers;
    std::unordered_map<uint32_t, std::unique_ptr<EBO>> indexBuffers;
    std::unordered_map<uint32_t, uint32_t> uniformBuffers;
    std::unordered_map<uint32_t, std::unique_ptr<VAO>> vertexArrays;
};