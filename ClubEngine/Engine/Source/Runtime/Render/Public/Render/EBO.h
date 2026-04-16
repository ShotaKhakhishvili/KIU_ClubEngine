#pragma once

#include <Render/RenderTypes.h>
#include <cstdint>
#include <cstddef>
#include <vector>

class EBO
{
public:
    EBO(const std::vector<uint32_t>& indices, BufferUsage usage = BufferUsage::StaticDraw);
    EBO(const void* data, size_t size, BufferUsage usage = BufferUsage::StaticDraw);
    ~EBO();

    void Bind() const;
    static void Unbind();

    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;

    EBO(EBO&&) noexcept;
    EBO& operator=(EBO&&) noexcept;

    EBOID GetID() const noexcept;

private:
    EBOID ID{0};
    
    void Delete();
};
