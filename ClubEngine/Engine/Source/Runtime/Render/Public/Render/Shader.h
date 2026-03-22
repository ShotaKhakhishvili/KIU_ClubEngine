#pragma once

#include <glad/glad.h>

class Shader
{
private:
    GLuint ID{0};

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Activate() const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    GLuint GetID() const noexcept;

private:
    void Delete();
};