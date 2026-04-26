#pragma once

#include <memory>
#include <unordered_map>

#include <RenderCore/RHI/IRHI.h>

class GLShader;
class GLTexture;
class GLVertexArray;
class GLVertexBuffer;
class GLIndexBuffer;

class OpenGLRHI final : public IRHI
{
public:
    void Init() override;
    void Shutdown() override;

    ShaderHandle CreateShader(const ShaderDesc& desc) override;
    void DestroyShader(ShaderHandle shader) override;

    TextureHandle CreateTexture(const TextureDesc& desc, const void* data) override;
    TextureHandle CreateCubemap(const TextureDesc& desc, const void* const* faceData) override;
    void DestroyTexture(TextureHandle texture) override;

    BufferHandle CreateBuffer(const BufferDesc& desc, const void* data) override;
    void DestroyBuffer(BufferHandle buffer) override;

    VertexArrayHandle CreateVertexArray(
        BufferHandle vertexBuffer,
        BufferHandle indexBuffer,
        const VertexArrayDesc& desc
    ) override;

    void DestroyVertexArray(VertexArrayHandle vertexArray) override;

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void SetClearColor(float r, float g, float b, float a) override;
    void Clear(bool color, bool depth) override;

    void SetCullMode(CullMode mode) override;
    void SetBlendMode(BlendMode mode) override;
    void SetDepthTest(bool enabled) override;
    void SetDepthWrite(bool enabled) override;

    void BindShader(ShaderHandle shader) override;
    void BindVertexArray(VertexArrayHandle vertexArray) override;
    void BindTexture(TextureHandle texture, uint32_t slot) override;

    void SetUniformBool(const std::string& name, bool value) override;
    void SetUniformInt(const std::string& name, int32_t value) override;
    void SetUniformFloat(const std::string& name, float value) override;
    void SetUniformVec2(const std::string& name, const Vec2f& value) override;
    void SetUniformVec3(const std::string& name, const Vec3f& value) override;
    void SetUniformVec4(const std::string& name, const Vec4f& value) override;

    void DrawIndexed(const DrawIndexedDesc& desc) override;

private:
    uint32_t nextShaderHandle = 1;
    uint32_t nextTextureHandle = 1;
    uint32_t nextBufferHandle = 1;
    uint32_t nextVertexArrayHandle = 1;

    ShaderHandle currentShader{};

    std::unordered_map<uint32_t, std::unique_ptr<GLShader>> shaders;
    std::unordered_map<uint32_t, std::unique_ptr<GLTexture>> textures;
    std::unordered_map<uint32_t, std::unique_ptr<GLVertexBuffer>> vertexBuffers;
    std::unordered_map<uint32_t, std::unique_ptr<GLIndexBuffer>> indexBuffers;
    std::unordered_map<uint32_t, uint32_t> uniformBuffers;
    std::unordered_map<uint32_t, std::unique_ptr<GLVertexArray>> vertexArrays;
};