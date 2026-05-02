#include "GLConvert.h"

#include <RHI.OpenGL/GLVertexBuffer.h>

#include <glad/glad.h>
#include <cassert>

namespace CE::RHI
{

GLVertexBuffer::GLVertexBuffer(const std::vector<Vertex>& vertices, BufferUsage usage)
    : GLVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), usage)
{
}

GLVertexBuffer::GLVertexBuffer(const void* data, size_t size, BufferUsage usage)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, ToGL(usage));
    
    #ifdef CE_DEBUG
        glObjectLabel(GL_BUFFER, ID, -1, "GLVertexBuffer");
    #endif
}

void GLVertexBuffer::Bind() const
{
    assert(ID != 0);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void GLVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBuffer::Delete()
{
    if (ID != 0)
    {
        glDeleteBuffers(1, &ID);
        ID = 0;
    }
}

VertexBufferID GLVertexBuffer::GetID() const noexcept
{
    return ID;
}

GLVertexBuffer::~GLVertexBuffer()
{
    Delete();
}

GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& other) noexcept
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