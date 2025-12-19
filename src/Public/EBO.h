#pragma once

#include <glad/glad.h>
#include "FuncLib.h"
#include <vector>

class EBO
{
    GLuint ID;

public:
    EBO(std::vector<GLuint>& indices);

    void Bind() const;
    static void Unbind();
    void Delete() const;

    GLuint getID() const;
};
