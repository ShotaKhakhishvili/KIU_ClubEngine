#include <Render/VBO.h>
#include "glad/glad.h"
#include <cassert>

VBO::VBO(const std::vector<Vertex>& vertices, BufferUsage usage)
    : VBO(vertices.data(), vertices.size() * sizeof(Vertex), usage)
{
}

VBO::VBO(const void* data, size_t size, BufferUsage usage)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
    
    #ifdef CE_DEBUG
        glObjectLabel(GL_BUFFER, ID, -1, "VBO");
    #endif
}

void VBO::Bind() const
{
    assert(ID != 0);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
    if (ID != 0)
    {
        glDeleteBuffers(1, &ID);
        ID = 0;
    }
}

VBOID VBO::GetID() const noexcept
{
    return ID;
}

VBO::~VBO()
{
    Delete();
}

VBO::VBO(VBO&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

VBO& VBO::operator=(VBO&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}