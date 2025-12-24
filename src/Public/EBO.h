#pragma once

#include <glad/glad.h>
#include "FuncLib.h"
#include <vector>

class EBO
{
    GLuint ID;

public:
    EBO(std::vector<GLuint>& indices);
    EBO(){}
    ~EBO();

    void Bind() const;
    static void Unbind();
    void Delete();

    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;

    GLuint getID() const;
};
