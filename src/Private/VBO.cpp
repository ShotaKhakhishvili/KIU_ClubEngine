#include "VBO.h"

VBO::VBO(std::vector<Vertex>& vertices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind() const
{
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

GLuint VBO::GetID() const
{
    return ID;
}

VBO::~VBO()
{
    Delete();
}