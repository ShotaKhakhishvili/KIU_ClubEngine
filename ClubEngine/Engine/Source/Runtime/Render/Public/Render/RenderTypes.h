#pragma once

#include <glad/glad.h>

using ShaderID = GLuint;
using TextureID = GLuint;
using VAOID = GLuint;
using VBOID = GLuint;
using EBOID = GLuint;

enum class ShaderDataType
{
    Float = GL_FLOAT,
    Int = GL_INT,
    Double = GL_DOUBLE
};

enum class DrawMode
{
    Triangles = GL_TRIANGLES,
    Lines = GL_LINES,
    LineStrip = GL_LINE_STRIP,
    LineLoop = GL_LINE_LOOP,
    Points = GL_POINTS
};

enum class BufferUsage
{
    StaticDraw = GL_STATIC_DRAW,
    DynamicDraw = GL_DYNAMIC_DRAW,
    StreamDraw = GL_STREAM_DRAW
};

enum class TextureType
{
    Texture2D = GL_TEXTURE_2D,
    TextureCubeMap = GL_TEXTURE_CUBE_MAP
};

enum class TextureSlot
{
    Slot0 = GL_TEXTURE0,
    Slot1 = GL_TEXTURE1,
    Slot2 = GL_TEXTURE2,
    Slot3 = GL_TEXTURE3,
    Slot4 = GL_TEXTURE4,
    Slot5 = GL_TEXTURE5,
    Slot6 = GL_TEXTURE6,
    Slot7 = GL_TEXTURE7,
    Slot8 = GL_TEXTURE8,
    Slot9 = GL_TEXTURE9,
    Slot10 = GL_TEXTURE10,
    Slot11 = GL_TEXTURE11,
    Slot12 = GL_TEXTURE12,
    Slot13 = GL_TEXTURE13,
    Slot14 = GL_TEXTURE14,
    Slot15 = GL_TEXTURE15
};

enum class TextureInternalFormat
{
    RGB8 = GL_RGB8,
    RGBA8 = GL_RGBA8,
    R8 = GL_R8,
    DepthComponent = GL_DEPTH_COMPONENT
};

enum class TextureFormat
{
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    Red = GL_RED,
    Depth = GL_DEPTH_COMPONENT
};

enum class TexturePixelType
{
    UnsignedByte = GL_UNSIGNED_BYTE,
    Float = GL_FLOAT
};

enum class CubemapFace
{
    Right = 0,
    Left,
    Top,
    Bottom,
    Front,
    Back
};