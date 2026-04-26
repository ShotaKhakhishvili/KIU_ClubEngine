#pragma once

#include <RenderCore/RenderTypes.h>

#include <cstdint>
#include <cstddef>

class GLVertexBuffer;

class GLVertexArray
{
public:

    ~GLVertexArray();
    GLVertexArray();

    void LinkAttrib(GLVertexBuffer& vbo, uint32_t layout, uint32_t componentCnt, ShaderDataType type, size_t stride, const void* offset) const;

    void Bind() const;
    static void Unbind();

    GLVertexArray(const GLVertexArray&) = delete;
    GLVertexArray& operator=(const GLVertexArray&) = delete;

    GLVertexArray(GLVertexArray&&) noexcept;
    GLVertexArray& operator=(GLVertexArray&&) noexcept;

    VertexArrayID GetID() const noexcept;

private:
    VertexArrayID ID{0};
    
    void Delete();
};
