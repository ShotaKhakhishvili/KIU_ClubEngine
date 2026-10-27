#pragma once

#include <Core/Types/FVertex.h>
#include <Core/Types/RHITypes.h>

#include <cstddef>
#include <vector>

namespace CE::RHI
{

class GLBuffer
{
public:
    GLBuffer(   const void* data, size_t inSize,
                BufferType type = BufferType::Vertex,
                BufferUsage usage = BufferUsage::StaticDraw
                );

    ~GLBuffer();

    void Bind() const;
    bool BindBase(uint32 slot) const;
    static void Unbind(BufferType bufferType);

    bool Read(void* dest, size_t readSize, size_t offset) const;
    bool Write(const void* data, size_t writeSize, size_t offset) const;

    GLBuffer(const GLBuffer&) = delete;
    GLBuffer& operator=(const GLBuffer&) = delete;

    GLBuffer(GLBuffer&& other) noexcept;
    GLBuffer& operator=(GLBuffer&& other) noexcept;

    BufferID GetID() const noexcept;

    BufferType GetType() const noexcept;

private:
    BufferID ID;
    size_t size;

    BufferType bufferType;

    void Delete();
};

}