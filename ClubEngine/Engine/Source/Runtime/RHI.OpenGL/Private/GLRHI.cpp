#include <Core/ClubCore.h>

#include "GLConvert.h"
#include <RHI.OpenGL/GLRHI.h>
#include <RHI.OpenGL/GLShaderBase.h>
#include <RHI.OpenGL/GLRenderTarget.h>
#include <RHI.OpenGL/GLTexture.h>
#include <RHI.OpenGL/GLVertexArray.h>
#include <RHI.OpenGL/GLBuffer.h>

#include <glad/glad.h>
#include <cassert>
#include <algorithm>

namespace CE::RHI
{

namespace
{

// Engine rectangles use a top-left origin; OpenGL window coordinates start bottom-left.
GLint FlipY(const ViewportDesc& rect, const uint32 targetHeight)
{
    return static_cast<GLint>(targetHeight) - static_cast<GLint>(rect.y) - static_cast<GLint>(rect.height);
}

uint32 BytesPerPixel(const TextureFormat format, const TexturePixelType type)
{
    uint32 components = 0;
    switch (format)
    {
    case TextureFormat::Red:   components = 1; break;
    case TextureFormat::RG:    components = 2; break;
    case TextureFormat::RGB:   components = 3; break;
    case TextureFormat::RGBA:  components = 4; break;
    case TextureFormat::Depth: components = 1; break;
    default: assert(false);
    }
    return components * (type == TexturePixelType::Float ? 4 : 1);
}

void FlipRows(uint8* data, const uint32 rowSize, const uint32 rowCount)
{
    for (uint32 i = 0; i < rowCount / 2; ++i)
        std::swap_ranges(data + i * rowSize, data + (i + 1) * rowSize,
                         data + (rowCount - 1 - i) * rowSize);
}

}

GLRHI::GLRHI() = default;
GLRHI::~GLRHI() = default;

void GLRHI::Init()
{
    gladLoadGL();
    GLint depthBits = 0;
    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

void GLRHI::Shutdown()
{
    shaders.clear();
    renderTargets.clear();
    textures.clear();
    buffers.clear();
    vertexArrays.clear();

    currentShader = {};
}

ShaderHandle GLRHI::CreateShader(const ShaderDesc& desc)
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
            case ShaderStage::Compute:
                auto shader = std::make_unique<GLShaderBase>(
                    ShaderDesc{
                        .stages = {{ShaderStage::Compute, src.source}}
                    });
                const uint32_t id = nextShaderHandle++;
                shaders[id] = std::move(shader);
                break;
        }
    }

    assert(!vertexShader.empty());
    assert(!fragmentShader.empty());

    ShaderDesc shaderDesc{
        .stages = {
            {ShaderStage::Vertex, vertexShader},
            {ShaderStage::Fragment, fragmentShader}
        }
	};

    auto shader = std::make_unique<GLShaderBase>(shaderDesc);

    if(!shader->IsValid())
    {
        CE_LOG(Error, "RHI.OpenGL: Shader Creation Failed");
        return ShaderHandle{};
    }
    
    const uint32_t id = nextShaderHandle++;

    shaders[id] = std::move(shader);

    return ShaderHandle{id};
}

void GLRHI::DestroyShader(ShaderHandle handle)
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

void GLRHI::BindShader(ShaderHandle handle)
{
    assert(handle.IsValid());

    auto it = shaders.find(handle.id);
    assert(it != shaders.end());

    it->second->Bind();
    currentShader = handle;
}

TextureHandle GLRHI::CreateTexture(const TextureDesc& desc, const void* data)
{
    assert(desc.dimension == RHI::TextureDimension::Texture2D);

    auto texture = std::make_unique<GLTexture>(desc, data);

    const uint32_t id = nextTextureHandle++;

    textures[id] = std::move(texture);

    return TextureHandle{id};
}

TextureHandle GLRHI::CreateCubemap(const RHI::TextureDesc& desc, const void* const* faceData)
{
    assert(desc.dimension == RHI::TextureDimension::TextureCube);

    auto texture = std::make_unique<GLTexture>(desc, faceData);

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

void GLRHI::BindTexture(TextureHandle handle, uint32_t slot)
{
    assert(handle.IsValid());

    auto it = textures.find(handle.id);
    assert(it != textures.end());

    it->second->Bind(slot);
}

BufferHandle GLRHI::CreateBuffer(const BufferDesc& desc, const void* data)
{
    const uint32_t id = nextBufferHandle++;

    auto buffer = std::make_unique<GLBuffer>(
        data,
        desc.size,
        desc.type,
        desc.usage
        );
    buffers[id] = std::move(buffer);

    return BufferHandle{id};
}

void GLRHI::DestroyBuffer(BufferHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Buffer");
        return;
    }

    buffers.erase(handle.id);
}

bool GLRHI::BindBufferBase(BufferHandle handle, uint32 bindingSlot)
{
    auto it = buffers.find(handle.id);
    assert(it != buffers.end());

    return it->second->BindBase(bindingSlot);
}

bool GLRHI::ReadBuffer(BufferHandle handle, void* dest, uint32 size, size_t offset)
{
    auto it = buffers.find(handle.id);
    assert(it != buffers.end());

    return it->second->Read(dest, size, offset);
}
bool GLRHI::WriteBuffer(BufferHandle handle, const void* data, const uint32 size, const size_t offset)
{
    auto it = buffers.find(handle.id);
    assert(it != buffers.end());

    return it->second->Write(data, size, offset);
}

void GLRHI::DispatchCompute(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
{

}
	
SyncFenceHandle GLRHI::CreateFence()                                                                  
{

}
bool GLRHI::PollFence(SyncFenceHandle handle)                                                
{
    
}
void GLRHI::DestroyFence(SyncFenceHandle handle)                                                
{

}

VertexArrayHandle GLRHI::CreateVertexArray(

    BufferHandle vertexBufferHandle,
    BufferHandle indexBufferHandle,
    const VertexArrayDesc& desc
)
{
    assert(vertexBufferHandle.IsValid());
    assert(indexBufferHandle.IsValid());

    auto VBOIt = buffers.find(vertexBufferHandle.id);
    auto EBOIt = buffers.find(indexBufferHandle.id);

    assert(VBOIt != buffers.end() && VBOIt->second->GetType() == BufferType::Vertex);
    assert(EBOIt != buffers.end() && EBOIt->second->GetType() == BufferType::Index);

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
    GLBuffer::Unbind(BufferType::Vertex);
    GLBuffer::Unbind(BufferType::Index);

    const uint32_t id = nextVertexArrayHandle++;

    vertexArrays[id] = std::move(vao);

    return VertexArrayHandle{id};
}

void GLRHI::DestroyVertexArray(VertexArrayHandle handle)
{
    if(!handle.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Unable To Access Vertex Array");
        return;
    }

    vertexArrays.erase(handle.id);
}

void GLRHI::BindVertexArray(VertexArrayHandle handle)
{
    assert(handle.IsValid());

    auto VAO = vertexArrays.find(handle.id);
    assert(VAO != vertexArrays.end());

    VAO->second->Bind();
}

void GLRHI::SetViewport(const ViewportDesc& desc)
{
    glViewport(
        static_cast<GLint>(desc.x),
        FlipY(desc, currentTargetHeight),
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

void GLRHI::SetCullMode(CullMode mode)
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

void GLRHI::SetBlendMode(BlendMode mode)
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

void GLRHI::SetUniformIVec2(const std::string& name, int x, int y)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetIVec2(name, x, y);
}

void GLRHI::SetUniformIVec3(const std::string& name, int x, int y, int z)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetIVec3(name, x, y, z);
}

void GLRHI::SetUniformIVec4(const std::string& name, int x, int y, int z, int w)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetIVec4(name, x, y, z, w);
}

void GLRHI::SetUniformVec2(const std::string& name, const FVector2F& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec2(name, value.x, value.y);
}

void GLRHI::SetUniformVec3(const std::string& name, const FVectorF& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec3(name, value.x, value.y, value.z);
}

void GLRHI::SetUniformVec4(const std::string& name, const FVector4F& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetVec4(name, value.x, value.y, value.z, value.w);
}

void GLRHI::SetUniformMat4(const std::string& name, const FMatrix4x4F& value)
{
    assert(currentShader.IsValid());
    shaders[currentShader.id]->SetMat4(name, value);
}

void GLRHI::SetTexture(const std::string& name, TextureHandle texture, uint32_t slot)
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

void GLRHI::DrawIndexed(const DrawIndexedDesc& desc)
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

GLTexture* GLRHI::FindTexture(const TextureHandle handle) const
{
    if (!handle.IsValid())
        return nullptr;

    const auto it = textures.find(handle.id);
    return it != textures.end() ? it->second.get() : nullptr;
}

RenderTargetHandle GLRHI::CreateRenderTarget(const RenderTargetDesc& desc)
{
    const GLTexture* colorTexture = FindTexture(desc.colorAttachment);
    const GLTexture* depthTexture = FindTexture(desc.depthAttachment);

    if (colorTexture == nullptr)
    {
        CE_LOG(Error, "RHI.OpenGL: Render Target Needs A Valid Color Attachment");
        return {};
    }

    if (desc.depthAttachment.IsValid() && depthTexture == nullptr)
    {
        CE_LOG(Error, "RHI.OpenGL: Render Target Has An Invalid Depth Attachment");
        return {};
    }

    if (depthTexture != nullptr &&
        (depthTexture->GetWidth() != colorTexture->GetWidth() || depthTexture->GetHeight() != colorTexture->GetHeight()))
    {
        CE_LOG(Error, "RHI.OpenGL: Render Target Attachments Must Have The Same Size");
        return {};
    }

    auto renderTarget = std::make_unique<GLRenderTarget>(*colorTexture, depthTexture);

    if (!renderTarget->IsComplete())
    {
        CE_LOG(Error, "RHI.OpenGL: Render Target Is Incomplete");
        return {};
    }

    const uint32 id = nextRenderTargetHandle++;
    renderTargets[id] = std::move(renderTarget);

    return RenderTargetHandle{id};
}

void GLRHI::DestroyRenderTarget(const RenderTargetHandle renderTarget)
{
    if (!renderTarget.IsValid())
    {
        CE_LOG(Warning, "RHI.OpenGL: Tried To Destroy An Invalid Render Target");
        return;
    }

    renderTargets.erase(renderTarget.id);
}

void GLRHI::BeginRenderPass(const RenderPassDesc& desc)
{
    if (desc.target.IsValid())
    {
        const auto it = renderTargets.find(desc.target.id);

        if (it == renderTargets.end())
        {
            CE_LOG(Error, "RHI.OpenGL: Tried To Begin A Render Pass On An Unknown Render Target");
            return;
        }

        it->second->Bind();
        currentTargetWidth = it->second->GetWidth();
        currentTargetHeight = it->second->GetHeight();
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        currentTargetWidth = backbufferWidth;
        currentTargetHeight = backbufferHeight;
    }

    inRenderPass = true;

    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, static_cast<GLsizei>(currentTargetWidth), static_cast<GLsizei>(currentTargetHeight));

    GLbitfield clearFlags = 0;

    if (desc.colorLoad == LoadOp::Clear)
    {
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClearColor(desc.clearColor.x, desc.clearColor.y, desc.clearColor.z, desc.clearColor.w);
        clearFlags |= GL_COLOR_BUFFER_BIT;
    }

    if (desc.depthLoad == LoadOp::Clear)
    {
        glDepthMask(GL_TRUE);
        glClearDepth(desc.clearDepth);
        clearFlags |= GL_DEPTH_BUFFER_BIT;
    }

    if (clearFlags != 0)
        glClear(clearFlags);

    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, static_cast<GLsizei>(currentTargetWidth), static_cast<GLsizei>(currentTargetHeight));
}

void GLRHI::EndRenderPass()
{
    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    inRenderPass = false;
}

void GLRHI::SetScissor(const ViewportDesc& desc)
{
    glScissor(
        static_cast<GLint>(desc.x),
        FlipY(desc, currentTargetHeight),
        static_cast<GLsizei>(desc.width),
        static_cast<GLsizei>(desc.height)
    );
}

void GLRHI::SetBackbufferSize(const uint32 width, const uint32 height)
{
    backbufferWidth = width;
    backbufferHeight = height;
}

void GLRHI::BlitToBackbuffer(const RenderTargetHandle renderTarget, const ViewportDesc& srcRect, const ViewportDesc& dstRect)
{
    const auto it = renderTargets.find(renderTarget.id);

    if (!renderTarget.IsValid() || it == renderTargets.end())
    {
        CE_LOG(Error, "RHI.OpenGL: Tried To Blit An Unknown Render Target");
        return;
    }

    const GLRenderTarget& source = *it->second;

    const GLint srcY = FlipY(srcRect, source.GetHeight());
    const GLint dstY = FlipY(dstRect, backbufferHeight);

    glDisable(GL_SCISSOR_TEST);     // the scissor clips blits too

    glBindFramebuffer(GL_READ_FRAMEBUFFER, source.GetID()); // Read from
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);              // Write to (id = 0 is the window's backbuffer's id)

    glBlitFramebuffer(
        static_cast<GLint>(srcRect.x), srcY,
        static_cast<GLint>(srcRect.x + srcRect.width), srcY + static_cast<GLint>(srcRect.height),
        static_cast<GLint>(dstRect.x), dstY,
        static_cast<GLint>(dstRect.x + dstRect.width), dstY + static_cast<GLint>(dstRect.height),
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool GLRHI::ReadTexture(TextureHandle handle, const ViewportDesc& rect, void* dest, const uint32 size)
{
    const GLTexture* texture = FindTexture(handle);
    if (!texture)
    {
        CE_LOG(Error, "ReadTexture: invalid texture handle [{}]", handle.id);
        return false;
    }

    if (texture->GetType() != TextureType::Texture2D)
        return false;

    if (rect.x + rect.width > texture->GetWidth() || rect.y + rect.height > texture->GetHeight())
        return false;

    const uint32 rowSize = rect.width * BytesPerPixel(texture->GetReadFormat(), texture->GetPixelType());
    if (size < rowSize * rect.height)
        return false;

    const GLint glY = FlipY(rect, texture->GetHeight());

    glGetTextureSubImage(texture->GetID(), 0,
                         rect.x, glY, 0, rect.width, rect.height, 1,
                         ToGL(texture->GetReadFormat()), ToGL(texture->GetPixelType()),
                         static_cast<GLsizei>(size), dest);

    FlipRows(static_cast<uint8*>(dest), rowSize, rect.height);
    return true;
}

}

