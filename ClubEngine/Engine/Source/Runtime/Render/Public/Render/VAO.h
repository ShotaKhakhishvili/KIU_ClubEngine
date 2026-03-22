#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <cstddef>

class VBO;

class VAO
{
private:
    GLuint ID{0};

public:

    ~VAO();
    VAO();

    void LinkAttrib(VBO& vbo, uint32_t layout, uint32_t componentCnt, GLenum type, size_t stride, const void* offset) const;

    void Bind() const;
    static void Unbind();

    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&&) noexcept;
    VAO& operator=(VAO&&) noexcept;

    GLuint GetID() const noexcept;

private:
    void Delete();
};
