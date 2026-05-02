#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace CE::RHI
{

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

constexpr uint32_t ToTextureUnitIndex(const TextureSlot slot) noexcept
{
    return static_cast<uint32_t>(slot);
}

enum class Backend
{
    OpenGL
};

enum class BufferType
{
    Vertex,
    Index,
    Uniform
};

enum class IndexType
{
    UInt16,
    UInt32
};

enum class PrimitiveType
{
    Triangles,
    Lines,
    Points
};

enum class TextureDimension
{
    Texture2D,
    TextureCube
};

enum class ShaderStage
{
    Vertex,
    Fragment
};

enum class UniformType
{
    Bool,
    Int,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat4
};

struct BufferDesc
{
    BufferType type = BufferType::Vertex;
    BufferUsage usage = BufferUsage::StaticDraw;
    uint32_t size = 0;
};

struct TextureDesc
{
    TextureDimension dimension = TextureDimension::Texture2D;

    uint32_t width = 0;
    uint32_t height = 0;

    TextureInternalFormat format = TextureInternalFormat::RGBA8;
    TextureFormat readFormat = TextureFormat::RGBA;
    TexturePixelType pixelType = TexturePixelType::UnsignedByte;

    bool generateMipmaps = true;
};

struct ShaderSource
{
    ShaderStage stage = ShaderStage::Vertex;
    std::string source;
};

struct ShaderDesc
{
    std::vector<ShaderSource> stages;
};

struct VertexAttributeDesc
{
    uint32_t location = 0;
    uint32_t componentCount = 0;
    ShaderDataType dataType = ShaderDataType::Float;

    uint32_t stride = 0;
    uint32_t offset = 0;
};

struct VertexArrayDesc
{
    std::vector<VertexAttributeDesc> attributes;
};

struct DrawIndexedDesc
{
    PrimitiveType primitiveType = PrimitiveType::Triangles;
    IndexType indexType = IndexType::UInt32;
        
    uint32_t indexCount = 0;
    uint32_t indexOffset = 0;
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

struct ViewportDesc
{
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t width = 0;
    uint32_t height = 0;
};

}