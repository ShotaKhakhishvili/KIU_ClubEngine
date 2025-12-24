#pragma once

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
    GLuint ID;

    public:

    ~VAO();
    VAO();
    void LinkAttrib(VBO& vbo, GLuint layout, GLuint componentCnt, GLenum type, GLsizeiptr stride, void* offset) const;

    void Bind() const;
    static void Unbind();
    void Delete();

    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    GLuint getID() const;
};
