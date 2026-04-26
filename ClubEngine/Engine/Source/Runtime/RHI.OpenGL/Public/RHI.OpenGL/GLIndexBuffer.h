#pragma once

#include <RenderCore/RenderTypes.h>

#include <cstdint>
#include <cstddef>
#include <vector>

class GLIndexBuffer
{
public:
    GLIndexBuffer(const std::vector<uint32_t>& indices, BufferUsage usage = BufferUsage::StaticDraw);
    GLIndexBuffer(const void* data, size_t size, BufferUsage usage = BufferUsage::StaticDraw);
    ~GLIndexBuffer();

    void Bind() const;
    static void Unbind();

    GLIndexBuffer(const GLIndexBuffer&) = delete;
    GLIndexBuffer& operator=(const GLIndexBuffer&) = delete;

    GLIndexBuffer(GLIndexBuffer&&) noexcept;
    GLIndexBuffer& operator=(GLIndexBuffer&&) noexcept;

    IndexBufferID GetID() const noexcept;

private:
    IndexBufferID ID{0};
    
    void Delete();
};
