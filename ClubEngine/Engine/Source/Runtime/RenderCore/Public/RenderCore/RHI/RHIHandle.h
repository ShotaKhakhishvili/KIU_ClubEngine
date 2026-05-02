#pragma once

#include <cstdint>

namespace CE::RHI
{
    template<typename T>
    class RHIHandle
    {
    public:
        uint32_t id{0};

        void Invalidate() noexcept
        {
            id = 0;
        }

        bool IsValid() const noexcept
        {
            return id != 0;
        }

        bool operator==(const RHIHandle& other)const noexcept
        {
            return this->id == other.id;
        }

        bool operator!=(const RHIHandle& other)const noexcept
        {
            return this->id != other.id;
        }
    };

    class RHIShaderTag{};
    class RHITextureTag{};
    class RHIBufferTag{};
    class RHIVertexArrayTag{};

    using ShaderHandle           = RHIHandle<RHIShaderTag>;
    using TextureHandle          = RHIHandle<RHITextureTag>;
    using BufferHandle           = RHIHandle<RHIBufferTag>;
    using VertexArrayHandle      = RHIHandle<RHIVertexArrayTag>;
}