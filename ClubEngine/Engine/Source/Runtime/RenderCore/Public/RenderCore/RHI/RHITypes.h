#pragma once

#include <RenderCore/RenderTypes.h>

#include <cstdint>
#include <string>
#include <vector>

enum class RHIBufferType
{
    Vertex,
    Index,
    Uniform
};

enum class RHIIndexType
{
    UInt16,
    UInt32
};

enum class RHIPrimitiveType
{
    Triangles,
    Lines,
    Points
};

enum class RHITextureDimension
{
    Texture2D,
    TextureCube
};

enum class RHIShaderStage
{
    Vertex,
    Fragment
};

enum class RHIUniformType
{
    Bool,
    Int,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat4
};

struct RHIBufferDesc
{
    RHIBufferType type = RHIBufferType::Vertex;
    BufferUsage usage = BufferUsage::StaticDraw;
    uint32_t size = 0;
};

struct RHITextureDesc
{
    RHITextureDimension dimension = RHITextureDimension::Texture2D;

    uint32_t width = 0;
    uint32_t height = 0;

    TextureInternalFormat format = TextureInternalFormat::RGBA8;
    TextureFormat readFormat = TextureFormat::RGBA;
    TexturePixelType pixelType = TexturePixelType::UnsignedByte;

    bool generateMipmaps = true;
};

struct RHIShaderSource
{
    RHIShaderStage stage = RHIShaderStage::Vertex;
    std::string source;
};

struct RHIShaderDesc
{
    std::vector<RHIShaderSource> stages;
};

struct RHIVertexAttributeDesc
{
    uint32_t location = 0;
    uint32_t componentCount = 0;
    ShaderDataType dataType = ShaderDataType::Float;

    uint32_t stride = 0;
    uint32_t offset = 0;
};

struct RHIVertexArrayDesc
{
    std::vector<RHIVertexAttributeDesc> attributes;
};

struct RHIDrawIndexedDesc
{
    RHIPrimitiveType primitiveType = RHIPrimitiveType::Triangles;
    RHIIndexType indexType = RHIIndexType::UInt32;
    
    uint32_t indexCount = 0;
    uint32_t indexOffset = 0;
};