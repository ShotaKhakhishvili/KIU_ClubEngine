#include "GLConvert.h"

#include <RHI.OpenGL/GLIndexBuffer.h>

#include <cassert>

namespace CE::RHI
{

GLIndexBuffer::GLIndexBuffer(const std::vector<uint32_t>& indices, BufferUsage usage)
    : GLIndexBuffer(indices.data(), indices.size() * sizeof(uint32_t), usage)
{
}

GLIndexBuffer::GLIndexBuffer(const void* data, size_t size, BufferUsage usage)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ToGL(usage));
    
    #ifdef CE_DEBUG
        glObjectLabel(GL_BUFFER, ID, -1, "GLIndexBuffer");
    #endif
}

void GLIndexBuffer::Bind() const
{
    assert(ID != 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void GLIndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLIndexBuffer::Delete()
{
    if (ID != 0)
    {
        glDeleteBuffers(1, &ID);
        ID = 0;
    }
}

IndexBufferID GLIndexBuffer::GetID() const noexcept
{
    return ID;
}

GLIndexBuffer::~GLIndexBuffer()
{
    Delete();
}

GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

GLIndexBuffer& GLIndexBuffer::operator=(GLIndexBuffer&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

}
