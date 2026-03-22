#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <cstddef>
#include <vector>

class EBO
{
private:
    GLuint ID{0};

public:
    EBO(const std::vector<uint32_t>& indices, GLenum usage = GL_STATIC_DRAW);
    EBO(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);
    ~EBO();

    void Bind() const;
    static void Unbind();

    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;

    EBO(EBO&&) noexcept;
    EBO& operator=(EBO&&) noexcept;

    GLuint GetID() const noexcept;

private:
    void Delete();
};
