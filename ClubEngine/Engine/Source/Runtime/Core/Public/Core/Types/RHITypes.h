#pragma once

#include <Core/Types/CoreTypes.h>
#include <Core/Math/Vector.h>

#include <string>
#include <vector>
#include <filesystem>

namespace CE::RHI
{

using ShaderID = uint32;
using TextureID = uint32;
using VertexArrayID = uint32;
using VertexBufferID = uint32;
using IndexBufferID = uint32;
using BufferID = uint32;
using RenderTargetID = uint32;

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
    RG8,
    DepthComponent,
    Count
};

enum class TextureFormat
{
    RGB,
    RGBA,
    Red,
    RG,
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

constexpr uint32 ToTextureUnitIndex(const TextureSlot slot) noexcept
{
    return static_cast<uint32>(slot);
}

enum class Backend
{
    OpenGL
};

enum class BufferType
{
    Vertex,
    Index,
    ShaderStorage,
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
    Fragment,
    Compute
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
    uint32 size = 0;
};

enum class TextureWrap
{
    Repeat,
    ClampToEdge,
    MirroredRepeat,
    Count
};

enum class TextureFilter
{
    Nearest,
    Linear,
    Count
};

struct TextureDesc
{
    TextureDimension dimension = TextureDimension::Texture2D;

    uint32 width = 0;
    uint32 height = 0;

    TextureInternalFormat format = TextureInternalFormat::RGBA8;
    TextureFormat readFormat = TextureFormat::RGBA;
    TexturePixelType pixelType = TexturePixelType::UnsignedByte;

    TextureWrap wrapU = TextureWrap::Repeat;
    TextureWrap wrapV = TextureWrap::Repeat;
    TextureFilter minFilter = TextureFilter::Nearest;
    TextureFilter magFilter = TextureFilter::Nearest;

    bool generateMipmaps = true;
    bool sRGB = false;
};

struct ShaderSource
{
    ShaderStage stage = ShaderStage::Vertex;
    std::string source;
};

struct ShaderFileSource
{
    ShaderStage stage = ShaderStage::Vertex;
    std::filesystem::path source;
};

struct ShaderDesc
{
    std::vector<ShaderSource> stages;
};

struct ShaderFileDesc
{
    std::vector<ShaderFileSource> stages;
};

struct VertexAttributeDesc
{
    uint32 location = 0;
    uint32 componentCount = 0;
    ShaderDataType dataType = ShaderDataType::Float;

    uint32 stride = 0;
    uint32 offset = 0;
};

struct VertexArrayDesc
{
    std::vector<VertexAttributeDesc> attributes;
};

struct DrawIndexedDesc
{
    PrimitiveType primitiveType = PrimitiveType::Triangles;
    IndexType indexType = IndexType::UInt32;
        
    uint32 indexCount = 0;
    uint32 indexOffset = 0;
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
    uint32 x = 0;
    uint32 y = 0;
    uint32 width = 0;
    uint32 height = 0;
};

enum class LoadOp
{
    Load,
    Clear,
    DontCare,
    Count
};

struct FRenderStateDesc
{
    BlendMode blendMode = BlendMode::Opaque;
    CullMode cullMode = CullMode::Back;
    bool depthWrite = true;
    bool depthTest = true;
};

enum class EMaterialTextureSlot : uint8
{
    BaseColor,
    MetallicRoughness,
    Normal,
    Occlusion,
    Emissive,
    Count
};

struct FMaterialSettings
{
    FVector4F baseColor{1.0f, 1.0f, 1.0f, 1.0f};
    float metallic = 0.0f;
    float roughness = 1.0f;
    FVectorF emissive{0.0f, 0.0f, 0.0f};

    float normalScale = 1.0f;
    float occlusionStrength = 1.0f;
    float alphaCutoff = 0.5f;
    bool unlit = false;

    FRenderStateDesc renderState;
};

}