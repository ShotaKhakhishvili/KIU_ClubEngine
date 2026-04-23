#pragma once

#include <RenderCore/RenderTypes.h>

#include <cstdint>
#include <cstddef>

class VBO;

class VAO
{
public:

    ~VAO();
    VAO();

    void LinkAttrib(VBO& vbo, uint32_t layout, uint32_t componentCnt, ShaderDataType type, size_t stride, const void* offset) const;

    void Bind() const;
    static void Unbind();

    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&&) noexcept;
    VAO& operator=(VAO&&) noexcept;

    VAOID GetID() const noexcept;

private:
    VAOID ID{0};
    
    void Delete();
};
