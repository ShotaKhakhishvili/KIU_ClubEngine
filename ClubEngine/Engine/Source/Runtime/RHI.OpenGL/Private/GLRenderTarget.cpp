#include <RHI.OpenGL/GLRenderTarget.h>
#include <RHI.OpenGL/GLTexture.h>

#include <glad/glad.h>

#include <cassert>

namespace CE::RHI
{

GLRenderTarget::GLRenderTarget(const GLTexture& colorTexture, const GLTexture* depthTexture)
    : width(colorTexture.GetWidth()), height(colorTexture.GetHeight())
{
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.GetID(), 0);

    if (depthTexture != nullptr)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetID(), 0);

    complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

#ifdef CE_DEBUG
    glObjectLabel(GL_FRAMEBUFFER, ID, -1, "GLRenderTarget");
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLRenderTarget::~GLRenderTarget()
{
    Delete();
}

GLRenderTarget::GLRenderTarget(GLRenderTarget&& other) noexcept
    : ID(other.ID), width(other.width), height(other.height), complete(other.complete)
{
    other.ID = 0;
}

GLRenderTarget& GLRenderTarget::operator=(GLRenderTarget&& other) noexcept
{
    if (this != &other)
    {
        Delete();

        ID = other.ID;
        width = other.width;
        height = other.height;
        complete = other.complete;

        other.ID = 0;
    }

    return *this;
}

void GLRenderTarget::Bind() const
{
    assert(ID != 0);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

bool GLRenderTarget::IsComplete() const noexcept
{
    return complete;
}

uint32 GLRenderTarget::GetID() const noexcept
{
    return ID;
}

uint32 GLRenderTarget::GetWidth() const noexcept
{
    return width;
}

uint32 GLRenderTarget::GetHeight() const noexcept
{
    return height;
}

void GLRenderTarget::Delete()
{
    if (ID != 0)
    {
        glDeleteFramebuffers(1, &ID);
        ID = 0;
    }
}

}