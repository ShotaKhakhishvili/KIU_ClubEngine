#pragma once

#include <glad/glad.h>

class VBO
{
    GLuint ID;
public:
    VBO(GLfloat* vertices, GLsizeiptr size);

    void Bind() const;
    static void Unbind();
    void Delete() const;

    GLuint GetID() const;
};
