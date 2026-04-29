#pragma once

#include <Core/Types/RenderTypes.h>

#include <cstdint>
#include <string>
#include <vector>

namespace RHI
{
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