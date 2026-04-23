#include "GLConvert.h"

#include <RHI.OpenGL/EBO.h>

#include <cassert>


EBO::EBO(const std::vector<uint32_t>& indices, BufferUsage usage)
    : EBO(indices.data(), indices.size() * sizeof(uint32_t), usage)
{
}

EBO::EBO(const void* data, size_t size, BufferUsage usage)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ToGL(usage));
    
    #ifdef CE_DEBUG
        glObjectLabel(GL_BUFFER, ID, -1, "EBO");
    #endif
}

void EBO::Bind() const
{
    assert(ID != 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
    if (ID != 0)
    {
        glDeleteBuffers(1, &ID);
        ID = 0;
    }
}

EBOID EBO::GetID() const noexcept
{
    return ID;
}

EBO::~EBO()
{
    Delete();
}

EBO::EBO(EBO&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

EBO& EBO::operator=(EBO&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}
