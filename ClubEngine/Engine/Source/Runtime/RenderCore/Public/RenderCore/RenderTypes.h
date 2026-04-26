#pragma once

#include <cstdint>

using ShaderID = uint32_t;
using TextureID = uint32_t;
using VertexArrayID = uint32_t;
using VertexBufferID = uint32_t;
using IndexBufferID = uint32_t;

enum class ShaderDataType
{
    Float,
    Int,
    Double,
    Count
};

enum class DrawMode
{
    Triangles,
    Lines,
    LineStrip,
    LineLoop,
    Points,
    Count
};

enum class BufferUsage
{
    StaticDraw,
    DynamicDraw,
    StreamDraw,
    Count
};

enum class TextureType
{
    Texture2D,
    TextureCubeMap,
    Count
};

enum class TextureSlot
{
    Slot0,
    Slot1,
    Slot2,
    Slot3,
    Slot4,
    Slot5,
    Slot6,
    Slot7,
    Slot8,
    Slot9,
    Slot10,
    Slot11,
    Slot12,
    Slot13,
    Slot14,
    Slot15,
    Count
};

enum class TextureInternalFormat
{
    RGB8,
    RGBA8,
    R8,
    DepthComponent,
    Count
};

enum class TextureFormat
{
    RGB,
    RGBA,
    Red,
    Depth,
    Count
};

enum class TexturePixelType
{
    UnsignedByte,
    Float,
    Count
};

enum class CubemapFace
{
    Right,
    Left,
    Top,
    Bottom,
    Front,
    Back,
    Count
};

enum class CullMode
{
    Back,
    Front,
    None,
    Count
};

enum class BlendMode
{
    Opaque,
    Masked,
    Translucent,
    Additive,
    Count
};

constexpr uint32_t ToTextureUnitIndex(const TextureSlot slot) noexcept
{
    return static_cast<uint32_t>(slot);
}