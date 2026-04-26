#include <Core/ClubCore.h>

#include "GLConvert.h"
#include <RHI.OpenGL/GLRHI.h>
#include <RHI.OpenGL/GLShader.h>
#include <RHI.OpenGL/GLTexture.h>
#include <RHI.OpenGL/GLVertexArray.h>
#include <RHI.OpenGL/GLVertexBuffer.h>
#include <RHI.OpenGL/GLIndexBuffer.h>

#include <glad/glad.h>
#include <cassert>

GLRHI::GLRHI() = default;
GLRHI::~GLRHI() = default;

void GLRHI::Init()
{
    glEnable(GL_DEPTH_TEST);
}

void GLRHI::Shutdown()
{
    shaders.clear();
    textures.clear();
    vertexBuffers.clear();
    indexBuffers.clear();
    vertexArrays.clear();

    currentShader = {};

    for (auto& [id, glBuffer] : uniformBuffers)
    {
        glDeleteBuffers(1, &glBuffer);
    }
    uniformBuffers.clear();
}

RHI::ShaderHandle GLRHI::CreateShader(const RHI::ShaderDesc& desc)
{
    std::string vertexShader;
    std::string fragmentShader;

    for(const auto& src : desc.stages)
    {
        switch(src.stage)
        {
            case RHI::ShaderStage::Vertex:
                vertexShader = src.source;
                break;
            case RHI::ShaderStage::Fragment:
                fragmentShader = src.source;
                break;
        }
    }

    assert(!vertexShader.empty());
    assert(!fragmentShader.empty());

    auto shader = std::make_unique<GLShader>(vertexShader.c_str(), fragmentShader.c_str());

    const uint32_t id = nextShaderHandle++;

    shaders[id] = std::move(shader);

    return RHI::ShaderHandle{id};
}

void GLRHI::DestroyShader(RHI::ShaderHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Shader");
        return;
    }

    shaders.erase(handle.id);

    if(handle == currentShader)
        currentShader = {};
}

void GLRHI::BindShader(RHI::ShaderHandle handle)
{
    assert(handle.IsValid());

    auto it = shaders.find(handle.id);
    assert(it != shaders.end());

    it->second->Bind();
    currentShader = handle;
}

RHI::TextureHandle GLRHI::CreateTexture(const RHI::TextureDesc& desc, const void* data)
{
    assert(desc.dimension == RHI::TextureDimension::Texture2D);

    auto texture = std::make_unique<GLTexture>(
                    desc.width,
                    desc.height,
                    desc.format,
                    desc.readFormat,
                    desc.pixelType,
                    desc.generateMipmaps,
                    data
    );

    const uint32_t id = nextTextureHandle++;

    textures[id] = std::move(texture);

    return RHI::TextureHandle{id};
}

RHI::TextureHandle GLRHI::CreateCubemap(const RHI::TextureDesc& desc, const void* const* faceData)
{
    assert(desc.dimension == RHI::TextureDimension::TextureCube);

    auto texture = std::make_unique<GLTexture>(
                    desc.width,
                    desc.height,
                    desc.format,
                    desc.readFormat,
                    desc.pixelType,
                    desc.generateMipmaps,
                    faceData
    );

    const uint32_t id = nextTextureHandle++;

    textures[id] = std::move(texture);

    return RHI::TextureHandle{id};
}

void GLRHI::DestroyTexture(RHI::TextureHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Texture");
        return;
    }

    textures.erase(handle.id);
}

void GLRHI::BindTexture(RHI::TextureHandle handle, uint32_t slot)
{
    assert(handle.IsValid());

    auto it = textures.find(handle.id);
    assert(it != textures.end());

    it->second->Bind(slot);
}

RHI::BufferHandle GLRHI::CreateBuffer(const RHI::BufferDesc& desc, const void* data)
{
    const uint32_t id = nextBufferHandle++;

    switch(desc.type)
    {
        case RHI::BufferType::Vertex:
        {
            auto buffer = std::make_unique<GLVertexBuffer>(
                data,
                desc.size,
                desc.usage
            );
            vertexBuffers[id] = std::move(buffer);
            break;
        }

        case RHI::BufferType::Index:
        {
            auto buffer = std::make_unique<GLIndexBuffer>(
                data,
                desc.size,
                desc.usage
            );
            indexBuffers[id] = std::move(buffer);
            break;
        }

        case RHI::BufferType::Uniform:
        {
            GLuint glBuffer = 0;
            glGenBuffers(1, &glBuffer);
            glBindBuffer(GL_UNIFORM_BUFFER, glBuffer);
            glBufferData(GL_UNIFORM_BUFFER, desc.size, data, ToGL(desc.usage));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            uniformBuffers[id] = glBuffer;
            break;
        }
    }

    return RHI::BufferHandle{id};
}

void GLRHI::DestroyBuffer(RHI::BufferHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Buffer");
        return;
    }

    vertexBuffers.erase(handle.id);
    indexBuffers.erase(handle.id);

    auto it = uniformBuffers.find(handle.id);
    if(it != uniformBuffers.end())
    {
        GLuint glBuffer = it->second;
        glDeleteBuffers(1, &glBuffer);
        uniformBuffers.erase(it);
    }
}

RHI::VertexArrayHandle GLRHI::CreateVertexArray(
    RHI::BufferHandle vertexBufferHandle,
    RHI::BufferHandle indexBufferHandle,
    const RHI::VertexArrayDesc& desc
)
{
    assert(vertexBufferHandle.IsValid());
    assert(indexBufferHandle.IsValid());

    auto VBOIt = vertexBuffers.find(vertexBufferHandle.id);
    auto EBOIt = indexBuffers.find(indexBufferHandle.id);

    assert(VBOIt != vertexBuffers.end());
    assert(EBOIt != indexBuffers.end());

    auto vao = std::make_unique<GLVertexArray>();

    vao->Bind();
    VBOIt->second->Bind();
    EBOIt->second->Bind();

    for(const auto& attribute : desc.attributes)
    {
        vao->LinkAttrib(
            *VBOIt->second,
            attribute.location,
            attribute.componentCount,
            attribute.dataType,
            attribute.stride,
            reinterpret_cast<const void*>(attribute.offset)
        );  
    }

    GLVertexArray::Unbind();
    GLVertexBuffer::Unbind();
    GLIndexBuffer::Unbind();

    const uint32_t id = nextVertexArrayHandle++;

    vertexArrays[id] = std::move(vao);

    return RHI::VertexArrayHandle{id};
}

void GLRHI::DestroyVertexArray(RHI::VertexArrayHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Vertex Array");
        return;
    }

    vertexArrays.erase(handle.id);
}

void GLRHI::BindVertexArray(RHI::VertexArrayHandle handle)
{
    assert(handle.IsValid());

    auto VAO = vertexArrays.find(handle.id);
    assert(VAO != vertexArrays.end());

    VAO->second->Bind();
}

void GLRHI::SetViewport(const RHI::ViewportDesc& desc)
{
    glViewport(
        static_cast<GLint>(desc.x),
        static_cast<GLint>(desc.y),
        static_cast<GLsizei>(desc.width),
        static_cast<GLsizei>(desc.height)
    );
}

void GLRHI::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void GLRHI::Clear(bool color, bool depth)
{
    GLbitfield flags = 0;

    if (color)
        flags |= GL_COLOR_BUFFER_BIT;

    if (depth)
        flags |= GL_DEPTH_BUFFER_BIT;

    glClear(flags);
}

void GLRHI::SetCullMode(RHI::CullMode mode)
{
    if (mode == RHI::CullMode::None)
    {
        glDisable(GL_CULL_FACE);
        return;
    }

    glEnable(GL_CULL_FACE);

    switch (mode)
    {
        case RHI::CullMode::Back:
            glCullFace(GL_BACK);
            break;

        case RHI::CullMode::Front:
            glCullFace(GL_FRONT);
            break;

        case RHI::CullMode::Count:
            assert(false && "Invalid CullMode");
            break;

        case RHI::CullMode::None:
            break;
    }
}

void GLRHI::SetBlendMode(RHI::BlendMode mode)
{
    switch (mode)
    {
        case RHI::BlendMode::Opaque:
            glDisable(GL_BLEND);
            break;

        case RHI::BlendMode::Masked:
            glDisable(GL_BLEND);
            break;

        case RHI::BlendMode::Translucent:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;

        case RHI::BlendMode::Additive:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        case RHI::BlendMode::Count:
            assert(false && "Invalid BlendMode");
            break;
    }
}

void GLRHI::SetDepthTest(bool enabled)
{
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void GLRHI::SetDepthWrite(bool enabled)
{
    glDepthMask(enabled ? GL_TRUE : GL_FALSE);
}

void GLRHI::SetUniformBool(const std::string& name, bool value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetBool(name, value);
}

void GLRHI::SetUniformInt(const std::string& name, int32_t value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetInt(name, value);
}

void GLRHI::SetUniformFloat(const std::string& name, float value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetFloat(name, value);
}

void GLRHI::SetUniformVec2(const std::string& name, const Vec2f& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec2(name, value.x, value.y);
}

void GLRHI::SetUniformVec3(const std::string& name, const Vec3f& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec3(name, value.x, value.y, value.z);
}

void GLRHI::SetUniformVec4(const std::string& name, const Vec4f& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec4(name, value.x, value.y, value.z, value.w);
}

void GLRHI::SetTexture(const std::string& name, RHI::TextureHandle texture, uint32_t slot)
{
    auto shaderIt = shaders.find(currentShader.id);
    if (shaderIt == shaders.end() || !shaderIt->second)
    {
        CE_LOG(Error, "Tried to set texture [{}] without valid bound shader", name);
        return;
    }

    auto textureIt = textures.find(texture.id);
    if (textureIt == textures.end() || !textureIt->second)
    {
        CE_LOG(Error, "Tried to set invalid texture [{}]", name);
        return;
    }

    BindTexture(texture, slot);

    shaderIt->second->SetInt(name, static_cast<int32_t>(slot));
}

void GLRHI::DrawIndexed(const RHI::DrawIndexedDesc& desc)
{
    const uint32_t indexSize =
    desc.indexType == RHI::IndexType::UInt16 ? sizeof(uint16_t) : sizeof(uint32_t);

    const uintptr_t byteOffset =
        static_cast<uintptr_t>(desc.indexOffset) * indexSize;

    glDrawElements(
        ToGL(desc.primitiveType),
        static_cast<GLsizei>(desc.indexCount),
        ToGL(desc.indexType),
        reinterpret_cast<const void*>(
            static_cast<uintptr_t>(byteOffset)
        )
    );
}