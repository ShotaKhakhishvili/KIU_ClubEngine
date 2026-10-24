#pragma once

#include <Core/Types/FVertex.h>
#include <Core/Types/RHITypes.h>

#include <cstddef>
#include <vector>

namespace CE::RHI
{

class GLVertexBuffer
{
public:
    GLVertexBuffer(const std::vector<FVertex>& vertices, BufferUsage usage = BufferUsage::StaticDraw);
    GLVertexBuffer(const void* data, size_t size, BufferUsage usage = BufferUsage::StaticDraw);

    ~GLVertexBuffer();

    void Bind() const;
    static void Unbind();

    GLVertexBuffer(const GLVertexBuffer&) = delete;
    GLVertexBuffer& operator=(const GLVertexBuffer&) = delete;

    GLVertexBuffer(GLVertexBuffer&& other) noexcept;
    GLVertexBuffer& operator=(GLVertexBuffer&& other) noexcept;

    VertexBufferID GetID() const noexcept;

private:
    VertexBufferID ID{0};
    
    void Delete();
};

}