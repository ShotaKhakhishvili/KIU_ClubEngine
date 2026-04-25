#include <Core/ClubCore.h>

#include "GLConvert.h"
#include <RHI.OpenGL/Shader.h>
#include <RHI.OpenGL/Texture.h>
#include <RHI.OpenGL/VAO.h>
#include <RHI.OpenGL/EBO.h>
#include <RHI.OpenGL/VBO.h>
#include <RHI.OpenGL/OpenGLRHI.h>

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
}

RHIShaderHandle OpenGLRHI::CreateShader(const RHIShaderDesc& desc)
{
    std::string vertexShader;
    std::string fragmentShader;

    for(auto src : desc.stages)
    {
        switch(src.stage)
        {
            case RHIShaderStage::Vertex:
                vertexShader = src.source;
                break;
            case RHIShaderStage::Fragment:
                fragmentShader = src.source;
                break;
        }
    }

    assert(!vertexShader.empty());
    assert(!fragmentShader.empty());

    auto shader = std::make_unique<Shader>(vertexShader.c_str(), fragmentShader.c_str());

    const uint32_t id = nextShaderHandle++;

    shaders[id] = std::move(shader);

    return RHIShaderHandle{id};
}

void OpenGLRHI::DestroyShader(RHIShaderHandle handle)
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

void OpenGLRHI::BindShader(RHIShaderHandle handle)
{
    assert(handle.IsValid());

    auto it = shaders.find(handle.id);
    assert(it != shaders.end());

    it->second->Bind();
    currentShader = handle;
}

RHITextureHandle OpenGLRHI::CreateTexture(const RHITextureDesc& desc, const void* data)
{
    assert(desc.dimension == RHITextureDimension::Texture2D);

    auto texture = std::make_unique<Texture>(
                    desc.width,
                    desc.height,
                    desc.format,
                    desc.readFormat,
                    desc.pixelType,
                    data
    );

    const uint32_t id = nextTextureHandle++;

    textures[id] = std::move(texture);

    return RHITextureHandle{id};
}

RHITextureHandle OpenGLRHI::CreateCubemap(const RHITextureDesc& desc, const void* const* faceData)
{
    assert(desc.dimension == RHITextureDimension::TextureCube);

    auto texture = std::make_unique<Texture>(
                    desc.width,
                    desc.height,
                    desc.format,
                    desc.readFormat,
                    desc.pixelType,
                    faceData
    );

    const uint32_t id = nextTextureHandle++;

    textures[id] = std::move(texture);

    return RHITextureHandle{id};
}

void OpenGLRHI::DestroyTexture(RHITextureHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Texture");
        return;
    }

    textures.erase(handle.id);
}

void OpenGLRHI::BindTexture(RHITextureHandle handle, uint32_t slot)
{
    assert(handle.IsValid());

    auto it = textures.find(handle.id);
    assert(it != textures.end());

    it->second->Bind(slot);
}

RHIBufferHandle OpenGLRHI::CreateBuffer(const RHIBufferDesc& desc, const void* data)
{
    const uint32_t id = nextBufferHandle++;

    switch(desc.type)
    {
        case RHIBufferType::Vertex:
        {
            auto buffer = std::make_unique<VBO>(
                data,
                desc.size,
                desc.usage
            );
            vertexBuffers[id] = std::move(buffer);
        }

        case RHIBufferType::Index:
        {
            auto buffer = std::make_unique<EBO>(
                data,
                desc.size,
                desc.usage
            );
            indexBuffers[id] = std::move(buffer);
        }

        case RHIBufferType::Uniform:
        {
            GLuint glBuffer = 0;
            glGenBuffers(1, &glBuffer);
            glBindBuffer(GL_UNIFORM_BUFFER, glBuffer);
            glBufferData(GL_UNIFORM_BUFFER, desc.size, data, ToGL(desc.usage));
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            uniformBuffers[id] = glBuffer;
        }
    }

    return RHIBufferHandle{id};
}

void OpenGLRHI::DestroyBuffer(RHIBufferHandle handle)
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

RHIVertexArrayHandle OpenGLRHI::CreateVertexArray(
    RHIBufferHandle vertexBufferHandle,
    RHIBufferHandle indexBufferHandle,
    const RHIVertexArrayDesc& desc
)
{
    assert(vertexBufferHandle.IsValid());
    assert(indexBufferHandle.IsValid());

    auto VBO = vertexBuffers.find(vertexBufferHandle.id);
    auto EBO = indexBuffers.find(indexBufferHandle.id);

    assert(VBO != vertexBuffers.end());
    assert(EBO != indexBuffers.end());

    auto vao = std::make_unique<VAO>();

    vao->Bind();
    VBO->second->Bind();
    EBO->second->Bind();

    for(const auto attribute : desc.attributes)
    {
        vao->LinkAttrib(
            *VBO->second,
            attribute.location,
            attribute.componentCount,
            attribute.dataType,
            attribute.stride,
            reinterpret_cast<const void*>(attribute.offset)
        );  
    }

    VAO::Unbind();
    VBO::Unbind();
    EBO::Unbind();

    const uint32_t id = nextVertexArrayHandle++;

    vertexArrays[id] = std::move(vao);

    return RHIVertexArrayHandle{id};
}

void OpenGLRHI::DestroyVertexArray(RHIVertexArrayHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Vertex Array");
        return;
    }

    vertexArrays.erase(handle.id);
}

void OpenGLRHI::BindVertexArray(RHIVertexArrayHandle handle)
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

        case BlendMode::Translucent:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;

        case BlendMode::Additive:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

void OpenGLRHI::DrawIndexed(const RHIDrawIndexedDesc& desc)
{
    glDrawElements(
        ToGL(desc.primitiveYype),
        static_cast<GLsizei>(desc.indexCount),
        ToGL(desc.indexType),
        reinterpret_cast<const void*>(
            static_cast<uintptr_t>(desc.indexOffset)
        )
    );
}