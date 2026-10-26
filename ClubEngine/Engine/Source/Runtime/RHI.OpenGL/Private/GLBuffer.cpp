#include "GLConvert.h"

#include <RHI.OpenGL/GLBuffer.h>

#include <glad/glad.h>
#include <cassert>

namespace CE::RHI
{

GLBuffer::GLBuffer(const void* data, const size_t inSize, const BufferType type, const BufferUsage usage)
    : size(inSize), bufferType(type)
{
    const GLenum target = ToGL(bufferType);

    if(bufferType == BufferType::Index)
        glBindVertexArray(0);

    glGenBuffers(1, &ID);
    glBindBuffer(target, ID);
    glBufferData(target, size, data, ToGL(usage));
    glBindBuffer(target, 0);

#ifdef CE_DEBUG
    glObjectLabel(GL_BUFFER, ID, -1, "GLBuffer");
#endif
}

void GLBuffer::Bind() const
{
    assert(ID != 0);
    glBindBuffer(ToGL(bufferType), ID);
}
bool GLBuffer::BindBase(const uint32 slot) const
{
    assert(ID != 0);

    if(bufferType != BufferType::Uniform && bufferType != BufferType::ShaderStorage)
        return false;

    glBindBufferBase(ToGL(bufferType), slot, ID);

    return true;
}

void GLBuffer::Unbind(BufferType bufferType)
{
    glBindBuffer(ToGL(bufferType), 0);
}
bool GLBuffer::Read(void* dest, size_t readSize, size_t offset) const
{
    assert(ID != 0);

    if (!dest || offset + readSize > size)
        return false;

    const GLenum target = ToGL(bufferType);

    if(bufferType == BufferType::Index)
        glBindVertexArray(0);

    glBindBuffer(target, ID);
    glBufferSubData(target, offset, readSize, dest);
    glBindBuffer(target, 0);

    return true;
}
bool GLBuffer::Write(const void* data, size_t writeSize, size_t offset) const
{
    assert(ID != 0);

    if (!data || offset + writeSize > size)
        return false;

    const GLenum target = ToGL(bufferType);

    if(bufferType == BufferType::Index)
        glBindVertexArray(0);

    glBindBuffer(target, ID);
    glBufferSubData(target, offset, writeSize, data);
    glBindBuffer(target, 0);

    return true;
}

void GLBuffer::Delete()
{
    if (ID != 0)
    {
        glDeleteBuffers(1, &ID);
        ID = 0;
    }
}

BufferID GLBuffer::GetID() const noexcept
{
    return ID;
}

GLBuffer::~GLBuffer()
{
    Delete();
}

GLBuffer::GLBuffer(GLBuffer&& other) noexcept
    : ID(other.ID), bufferType(other.bufferType)
{
    other.ID = 0;
}

GLBuffer& GLBuffer::operator=(GLBuffer&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        ID = other.ID;
        bufferType = other.bufferType;
        other.ID = 0;
    }
    return *this;
}

}