#include "GLConvert.h"

#include <cassert>

GLenum ToGL(ShaderDataType v) noexcept
{
    switch (v)
    {
        case ShaderDataType::Float:  return GL_FLOAT;
        case ShaderDataType::Int:    return GL_INT;
        case ShaderDataType::Double: return GL_DOUBLE;
        case ShaderDataType::Count:  break;
    }

    assert(false && "Unhandled ShaderDataType");
    return GL_FLOAT;
}

GLenum ToGL(BufferUsage v) noexcept
{
    switch (v)
    {
        case BufferUsage::StaticDraw:  return GL_STATIC_DRAW;
        case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
        case BufferUsage::StreamDraw:  return GL_STREAM_DRAW;
        case BufferUsage::Count:       break;
    }

    assert(false && "Unhandled BufferUsage");
    return GL_STATIC_DRAW;
}

GLenum ToGL(TextureType v) noexcept
{
    switch (v)
    {
        case TextureType::Texture2D:      return GL_TEXTURE_2D;
        case TextureType::TextureCubeMap: return GL_TEXTURE_CUBE_MAP;
        case TextureType::Count:          break;
    }

    assert(false && "Unhandled TextureType");
    return GL_TEXTURE_2D;
}

GLenum ToGL(TextureSlot v) noexcept
{
    switch (v)
    {
        case TextureSlot::Slot0:  return GL_TEXTURE0;
        case TextureSlot::Slot1:  return GL_TEXTURE1;
        case TextureSlot::Slot2:  return GL_TEXTURE2;
        case TextureSlot::Slot3:  return GL_TEXTURE3;
        case TextureSlot::Slot4:  return GL_TEXTURE4;
        case TextureSlot::Slot5:  return GL_TEXTURE5;
        case TextureSlot::Slot6:  return GL_TEXTURE6;
        case TextureSlot::Slot7:  return GL_TEXTURE7;
        case TextureSlot::Slot8:  return GL_TEXTURE8;
        case TextureSlot::Slot9:  return GL_TEXTURE9;
        case TextureSlot::Slot10: return GL_TEXTURE10;
        case TextureSlot::Slot11: return GL_TEXTURE11;
        case TextureSlot::Slot12: return GL_TEXTURE12;
        case TextureSlot::Slot13: return GL_TEXTURE13;
        case TextureSlot::Slot14: return GL_TEXTURE14;
        case TextureSlot::Slot15: return GL_TEXTURE15;
        case TextureSlot::Count:  break;
    }

    assert(false && "Unhandled TextureSlot");
    return GL_TEXTURE0;
}

GLenum ToGL(TextureInternalFormat v) noexcept
{
    switch (v)
    {
        case TextureInternalFormat::RGB8:           return GL_RGB8;
        case TextureInternalFormat::RGBA8:          return GL_RGBA8;
        case TextureInternalFormat::R8:             return GL_R8;
        case TextureInternalFormat::DepthComponent: return GL_DEPTH_COMPONENT;
        case TextureInternalFormat::Count:          break;
    }

    assert(false && "Unhandled TextureInternalFormat");
    return GL_RGBA8;
}

GLenum ToGL(TextureFormat v) noexcept
{
    switch (v)
    {
        case TextureFormat::RGB:   return GL_RGB;
        case TextureFormat::RGBA:  return GL_RGBA;
        case TextureFormat::Red:   return GL_RED;
        case TextureFormat::Depth: return GL_DEPTH_COMPONENT;
        case TextureFormat::Count: break;
    }

    assert(false && "Unhandled TextureFormat");
    return GL_RGBA;
}

GLenum ToGL(TexturePixelType v) noexcept
{
    switch (v)
    {
        case TexturePixelType::UnsignedByte: return GL_UNSIGNED_BYTE;
        case TexturePixelType::Float:        return GL_FLOAT;
        case TexturePixelType::Count:        break;
    }

    assert(false && "Unhandled TexturePixelType");
    return GL_UNSIGNED_BYTE;
}