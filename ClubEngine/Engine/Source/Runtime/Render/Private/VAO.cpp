#include <Render/VAO.h>
#include <Render/VBO.h>
#include <glad/glad.h>
#include <cassert>

VAO::VAO()
{
    glGenVertexArrays(1, &ID);

    #ifdef CE_DEBUG
        glObjectLabel(GL_VERTEX_ARRAY, ID, -1, "VAO");
    #endif
}

void VAO::LinkAttrib(VBO& vbo, uint32_t layout, uint32_t componentCnt, ShaderDataType type, size_t stride, const void* offset) const
{
    assert(ID != 0);

    Bind();
    vbo.Bind();
    glVertexAttribPointer(layout, componentCnt, static_cast<GLenum>(type), GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    //vbo.Unbind();
}

void VAO::Bind() const
{
    assert(ID != 0);
    glBindVertexArray(ID);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::Delete()
{
    if (ID != 0)
    {
        glDeleteVertexArrays(1, &ID);
        ID = 0;
    }
}

VAOID VAO::GetID() const noexcept
{
    return ID;
}

VAO::~VAO()
{
    Delete();
}

VAO::VAO(VAO&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

VAO& VAO::operator=(VAO&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}