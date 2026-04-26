#include <Core/ClubCore.h>

#include "GLConvert.h"
#include <RHI.OpenGL/GLShader.h>
#include <RHI.OpenGL/GLTexture.h>
#include <RHI.OpenGL/GLVertexBuffer.h>
#include <RHI.OpenGL/GLIndexBuffer.h>
#include <RHI.OpenGL/GLVertexArray.h>
#include <RHI.OpenGL/GLRHI.h>

#include <glad/glad.h>
#include <cassert>

void OpenGLRHI::Init()
{
    glEnable(GL_DEPTH_TEST);
}

void OpenGLRHI::Shutdown()
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

ShaderHandle OpenGLRHI::CreateShader(const ShaderDesc& desc)
{
    std::string vertexShader;
    std::string fragmentShader;

    for(const auto& src : desc.stages)
    {
        switch(src.stage)
        {
            case ShaderStage::Vertex:
                vertexShader = src.source;
                break;
            case ShaderStage::Fragment:
                fragmentShader = src.source;
                break;
        }
    }

    assert(!vertexShader.empty());
    assert(!fragmentShader.empty());

    auto shader = std::make_unique<GLShader>(vertexShader.c_str(), fragmentShader.c_str());

    const uint32_t id = nextShaderHandle++;

    shaders[id] = std::move(shader);

    return ShaderHandle{id};
}

void OpenGLRHI::DestroyShader(ShaderHandle handle)
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

void OpenGLRHI::BindShader(ShaderHandle handle)
{
    assert(handle.IsValid());

    auto it = shaders.find(handle.id);
    assert(it != shaders.end());

    it->second->Bind();
    currentShader = handle;
}

TextureHandle OpenGLRHI::CreateTexture(const TextureDesc& desc, const void* data)
{
    assert(desc.dimension == TextureDimension::Texture2D);

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

    return TextureHandle{id};
}

TextureHandle OpenGLRHI::CreateCubemap(const TextureDesc& desc, const void* const* faceData)
{
    assert(desc.dimension == TextureDimension::TextureCube);

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

    return TextureHandle{id};
}

void OpenGLRHI::DestroyTexture(TextureHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Texture");
        return;
    }

    textures.erase(handle.id);
}

void OpenGLRHI::BindTexture(TextureHandle handle, uint32_t slot)
{
    assert(handle.IsValid());

    auto it = textures.find(handle.id);
    assert(it != textures.end());

    it->second->Bind(slot);
}

BufferHandle OpenGLRHI::CreateBuffer(const BufferDesc& desc, const void* data)
{
    const uint32_t id = nextBufferHandle++;

    switch(desc.type)
    {
        case BufferType::Vertex:
        {
            auto buffer = std::make_unique<GLVertexBuffer>(
                data,
                desc.size,
                desc.usage
            );
            vertexBuffers[id] = std::move(buffer);
            break;
        }

        case BufferType::Index:
        {
            auto buffer = std::make_unique<GLIndexBuffer>(
                data,
                desc.size,
                desc.usage
            );
            indexBuffers[id] = std::move(buffer);
            break;
        }

        case BufferType::Uniform:
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

    return BufferHandle{id};
}

void OpenGLRHI::DestroyBuffer(BufferHandle handle)
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

VertexArrayHandle OpenGLRHI::CreateVertexArray(
    BufferHandle vertexBufferHandle,
    BufferHandle indexBufferHandle,
    const VertexArrayDesc& desc
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

    return VertexArrayHandle{id};
}

void OpenGLRHI::DestroyVertexArray(VertexArrayHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Vertex Array");
        return;
    }

    vertexArrays.erase(handle.id);
}

void OpenGLRHI::BindVertexArray(VertexArrayHandle handle)
{
    assert(handle.IsValid());

    auto VAO = vertexArrays.find(handle.id);
    assert(VAO != vertexArrays.end());

    VAO->second->Bind();
}

void OpenGLRHI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(
        static_cast<GLint>(x),
        static_cast<GLint>(y),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height)
    );
}

void OpenGLRHI::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void OpenGLRHI::Clear(bool color, bool depth)
{
    GLbitfield flags = 0;

    if (color)
        flags |= GL_COLOR_BUFFER_BIT;

    if (depth)
        flags |= GL_DEPTH_BUFFER_BIT;

    glClear(flags);
}

void OpenGLRHI::SetCullMode(CullMode mode)
{
    if (mode == CullMode::None)
    {
        glDisable(GL_CULL_FACE);
        return;
    }

    glEnable(GL_CULL_FACE);

    switch (mode)
    {
        case CullMode::Back:
            glCullFace(GL_BACK);
            break;

        case CullMode::Front:
            glCullFace(GL_FRONT);
            break;

        case CullMode::Count:
            assert(false && "Invalid CullMode");
            break;

        case CullMode::None:
            break;
    }
}

void OpenGLRHI::SetBlendMode(BlendMode mode)
{
    switch (mode)
    {
        case BlendMode::Opaque:
            glDisable(GL_BLEND);
            break;

        case BlendMode::Masked:
            glDisable(GL_BLEND);
            break;

        case BlendMode::Translucent:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;

        case BlendMode::Additive:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        case BlendMode::Count:
            assert(false && "Invalid BlendMode");
            break;
    }
}

void OpenGLRHI::SetDepthTest(bool enabled)
{
    if (enabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OpenGLRHI::SetDepthWrite(bool enabled)
{
    glDepthMask(enabled ? GL_TRUE : GL_FALSE);
}

void OpenGLRHI::SetUniformBool(const std::string& name, bool value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetBool(name, value);
}

void OpenGLRHI::SetUniformInt(const std::string& name, int32_t value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetInt(name, value);
}

void OpenGLRHI::SetUniformFloat(const std::string& name, float value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetFloat(name, value);
}

void OpenGLRHI::SetUniformVec2(const std::string& name, const Vec2f& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec2(name, value.x, value.y);
}

void OpenGLRHI::SetUniformVec3(const std::string& name, const Vec3f& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec3(name, value.x, value.y, value.z);
}

void OpenGLRHI::SetUniformVec4(const std::string& name, const Vec4f& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec4(name, value.x, value.y, value.z, value.w);
}

void OpenGLRHI::DrawIndexed(const DrawIndexedDesc& desc)
{
    const uint32_t indexSize =
    desc.indexType == IndexType::UInt16 ? sizeof(uint16_t) : sizeof(uint32_t);

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