#pragma once

#include <glad/glad.h>

class EBO
{
    GLuint ID;

public:
    EBO(GLuint* indices, GLsizeiptr size);

    void Bind() const;
    static void Unbind();
    void Delete() const;

    GLuint getID() const;
};
