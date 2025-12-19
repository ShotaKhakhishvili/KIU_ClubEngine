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

    void Bind() const;
    static void Unbind();
    void Delete() const;

    GLuint GetID() const;
};
