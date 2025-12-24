#pragma once

#include <glad/glad.h>
#include "FuncLib.h"
#include <vector>

class VBO
{
    GLuint ID;
public:
    VBO(std::vector<Vertex>& vertices);
    VBO(){}
    ~VBO();

    void Bind() const;
    static void Unbind();
    void Delete();

    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

    GLuint GetID() const;
};
