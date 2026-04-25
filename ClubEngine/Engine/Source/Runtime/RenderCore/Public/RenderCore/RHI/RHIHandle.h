#pragma once

#include <cstdint>

template<typename T>
class RHIHandle
{
public:
    uint32_t id{0};

    bool IsValid() const noexcept
    {
        return id != 0;
    }

    bool operator==(const RHIHandle& other)const
    {
        return this->id == other.id;
    }

    bool operator!=(const RHIHandle& other)const
    {
        return this->id != other.id;
    }
};

class RHIShaderTag{};
class RHITextureTag{};
class RHIBufferTag{};
class RHIVertexArrayTag{};

using RHIShaderHandle           = RHIHandle<RHIShaderTag>;
using RHITextureHandle          = RHIHandle<RHITextureTag>;
using RHIBufferHandle           = RHIHandle<RHIBufferTag>;
using RHIVertexArrayHandle      = RHIHandle<RHIVertexArrayTag>;