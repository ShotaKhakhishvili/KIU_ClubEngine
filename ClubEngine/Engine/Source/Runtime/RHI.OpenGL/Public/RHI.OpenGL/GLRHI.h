#pragma once

#include <RenderCore/RHI/IRHI.h>

#include <memory>
#include <unordered_map>

namespace CE::RHI
{

class GLShader;
class GLTexture;
class GLVertexArray;
class GLVertexBuffer;
class GLIndexBuffer;

class GLRHI final : public IRHI
{
public:
    GLRHI();
    ~GLRHI() override;

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

    void SetViewport(const ViewportDesc& desc) override;
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
    void SetUniformVec2(const std::string& name, const CE::FVector2F& value) override;
    void SetUniformVec3(const std::string& name, const CE::FVectorF& value) override;
    void SetUniformVec4(const std::string& name, const CE::FVector4F& value) override;
    void SetTexture(const std::string& name, TextureHandle texture, uint32_t slot) override;
    
    void DrawIndexed(const DrawIndexedDesc& desc) override;

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

}