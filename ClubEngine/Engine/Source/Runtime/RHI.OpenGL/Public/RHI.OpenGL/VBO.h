#pragma once

#include <RenderCore/Vertex.h>
#include <RenderCore/RenderTypes.h>

#include <cstddef>
#include <vector>

class VBO
{
public:
    VBO(const std::vector<Vertex>& vertices, BufferUsage usage = BufferUsage::StaticDraw);
    VBO(const void* data, size_t size, BufferUsage usage = BufferUsage::StaticDraw);

    ~VBO();

    void Bind() const;
    static void Unbind();

    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

    VBO(VBO&& other) noexcept;
    VBO& operator=(VBO&& other) noexcept;

    VBOID GetID() const noexcept;

private:
    VBOID ID{0};
    
    void Delete();
};