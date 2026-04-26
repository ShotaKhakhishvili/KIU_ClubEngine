#pragma once

#include <RenderCore/RHI/IRHI.h>

#include <memory>
#include <unordered_map>

class GLShader;
class GLTexture;
class GLVertexArray;
class GLVertexBuffer;
class GLIndexBuffer;

class GLRHI final : public RHI::IRHI
{
public:
    GLRHI();
    ~GLRHI() override;

    void Init() override;
    void Shutdown() override;

    RHI::ShaderHandle CreateShader(const RHI::ShaderDesc& desc) override;
    void DestroyShader(RHI::ShaderHandle shader) override;

    RHI::TextureHandle CreateTexture(const RHI::TextureDesc& desc, const void* data) override;
    RHI::TextureHandle CreateCubemap(const RHI::TextureDesc& desc, const void* const* faceData) override;
    void DestroyTexture(RHI::TextureHandle texture) override;

    RHI::BufferHandle CreateBuffer(const RHI::BufferDesc& desc, const void* data) override;
    void DestroyBuffer(RHI::BufferHandle buffer) override;

    RHI::VertexArrayHandle CreateVertexArray(
        RHI::BufferHandle vertexBuffer,
        RHI::BufferHandle indexBuffer,
        const RHI::VertexArrayDesc& desc
    ) override;

    void DestroyVertexArray(RHI::VertexArrayHandle vertexArray) override;

    void SetViewport(const RHI::ViewportDesc& desc) override;
    void SetClearColor(float r, float g, float b, float a) override;
    void Clear(bool color, bool depth) override;

    void SetCullMode(RHI::CullMode mode) override;
    void SetBlendMode(RHI::BlendMode mode) override;
    void SetDepthTest(bool enabled) override;
    void SetDepthWrite(bool enabled) override;

    void BindShader(RHI::ShaderHandle shader) override;
    void BindVertexArray(RHI::VertexArrayHandle vertexArray) override;
    void BindTexture(RHI::TextureHandle texture, uint32_t slot) override;

    void SetUniformBool(const std::string& name, bool value) override;
    void SetUniformInt(const std::string& name, int32_t value) override;
    void SetUniformFloat(const std::string& name, float value) override;
    void SetUniformVec2(const std::string& name, const Vec2f& value) override;
    void SetUniformVec3(const std::string& name, const Vec3f& value) override;
    void SetUniformVec4(const std::string& name, const Vec4f& value) override;
    void SetTexture(const std::string& name, RHI::TextureHandle texture, uint32_t slot) override;
    
    void DrawIndexed(const RHI::DrawIndexedDesc& desc) override;

private:
    uint32_t nextShaderHandle = 1;
    uint32_t nextTextureHandle = 1;
    uint32_t nextBufferHandle = 1;
    uint32_t nextVertexArrayHandle = 1;

    RHI::ShaderHandle currentShader{};

    std::unordered_map<uint32_t, std::unique_ptr<GLShader>> shaders;
    std::unordered_map<uint32_t, std::unique_ptr<GLTexture>> textures;
    std::unordered_map<uint32_t, std::unique_ptr<GLVertexBuffer>> vertexBuffers;
    std::unordered_map<uint32_t, std::unique_ptr<GLIndexBuffer>> indexBuffers;
    std::unordered_map<uint32_t, uint32_t> uniformBuffers;
    std::unordered_map<uint32_t, std::unique_ptr<GLVertexArray>> vertexArrays;
};