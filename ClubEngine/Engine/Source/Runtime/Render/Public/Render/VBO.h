#pragma once

#include <glad/glad.h>
#include <cstddef>
#include <vector>
#include <Render/Mesh/Vertex.h>

class VBO
{
    GLuint ID{0};

public:
    VBO(const std::vector<Vertex>& vertices, GLenum usage = GL_STATIC_DRAW);
    VBO(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);

    ~VBO();

    void Bind() const;
    static void Unbind();

    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

    VBO(VBO&& other) noexcept;
    VBO& operator=(VBO&& other) noexcept;

    GLuint GetID() const noexcept;

private:
    void Delete();
};