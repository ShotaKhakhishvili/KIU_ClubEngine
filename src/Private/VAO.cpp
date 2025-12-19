#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& vbo, GLuint layout, GLuint componentCnt, GLenum type, GLsizeiptr stride, void* offset) const
{
    vbo.Bind();
    glVertexAttribPointer(layout, componentCnt, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    vbo.Unbind();
}


void VAO::Bind() const
{
    glBindVertexArray(ID);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::Delete() const
{
    glDeleteVertexArrays(1, &ID);
}

GLuint VAO::getID() const
{
    return ID;
}
