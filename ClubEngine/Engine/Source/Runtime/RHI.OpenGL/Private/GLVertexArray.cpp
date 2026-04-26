#include "GLConvert.h"

#include <RHI.OpenGL/GLVertexArray.h>
#include <RHI.OpenGL/GLVertexBuffer.h>

#include <glad/glad.h>
#include <cassert>

GLVertexArray::GLVertexArray()
{
    glGenVertexArrays(1, &ID);

    #ifdef CE_DEBUG
        glObjectLabel(GL_VERTEX_ARRAY, ID, -1, "GLVertexArray");
    #endif
}

void GLVertexArray::LinkAttrib(GLVertexBuffer& vbo, uint32_t layout, uint32_t componentCnt, ShaderDataType type, size_t stride, const void* offset) const
{
    assert(ID != 0);

    Bind();
    vbo.Bind();
    glVertexAttribPointer(layout, componentCnt, ToGL(type), GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
}

void GLVertexArray::Bind() const
{
    assert(ID != 0);
    glBindVertexArray(ID);
}

void GLVertexArray::Unbind()
{
    glBindVertexArray(0);
}

void GLVertexArray::Delete()
{
    if (ID != 0)
    {
        glDeleteVertexArrays(1, &ID);
        ID = 0;
    }
}

VertexArrayID GLVertexArray::GetID() const noexcept
{
    return ID;
}

GLVertexArray::~GLVertexArray()
{
    Delete();
}

GLVertexArray::GLVertexArray(GLVertexArray&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

GLVertexArray& GLVertexArray::operator=(GLVertexArray&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}