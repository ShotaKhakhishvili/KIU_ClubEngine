#pragma once

#include <Render/RenderTypes.h>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Activate() const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    ShaderID GetID() const noexcept;

private:
    ShaderID ID{0};

    void Delete();
};