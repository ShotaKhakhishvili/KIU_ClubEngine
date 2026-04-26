#include "GLConvert.h"

#include <Core/ClubCore.h>

#include <RHI.OpenGL/GLTexture.h>

#include <cassert>

GLTexture::GLTexture(
        const uint32_t width, 
        const uint32_t height, 
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType,
        bool generateMipmaps,
        const void* data
    )
    :   type(TextureType::Texture2D), 
        internalFormat(internalFormat), 
        readFormat(readFormat), 
        pixelType(pixelType),
        generateMipmaps(generateMipmaps),
        width(width),
        height(height)
{
    assert(type == TextureType::Texture2D);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(ToGL(type), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(ToGL(type), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(ToGL(type), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(ToGL(type), GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        ToGL(type),
        0,
        ToGL(internalFormat),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0,
        ToGL(readFormat),
        ToGL(pixelType),
        data
    );

    if(generateMipmaps)
        GenerateMipmap();

    glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::GLTexture(
        const uint32_t width,
        const uint32_t height,
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType,
        bool generateMipmaps,
        const void* const* data
    )
    :   type(TextureType::TextureCubeMap),
        internalFormat(internalFormat),
        readFormat(readFormat),
        pixelType(pixelType),
        generateMipmaps(generateMipmaps),
        width(width),
        height(height)
{
    assert(type == TextureType::TextureCubeMap);
    assert(data != nullptr);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    for (size_t i = 0; i < 6; ++i)
    {
        assert(data[i] != nullptr);

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i),
            0,
            ToGL(internalFormat),
            static_cast<GLsizei>(width),
            static_cast<GLsizei>(height),
            0,
            ToGL(readFormat),
            ToGL(pixelType),
            data[i]
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if(generateMipmaps)
        GenerateMipmap();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GLTexture::Bind(int32_t slot) const
{
    assert(ID != 0);
    assert(slot >= 0);

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(ToGL(type), ID);
}

void GLTexture::Unbind() const
{
    glBindTexture(ToGL(type), 0);
}

void GLTexture::Delete()
{
    if (ID != 0)
    {
        glDeleteTextures(1, &ID);
        ID = 0;
    }
}

GLTexture::~GLTexture()
{
    Delete();
}

GLTexture::GLTexture(GLTexture&& other) noexcept
    :   ID(other.ID), 
        type(other.type), 
        internalFormat(other.internalFormat),
        readFormat(other.readFormat), 
        pixelType(other.pixelType), 
        width(other.width), 
        height(other.height)
{
    other.ResetToDefault();
}

GLTexture& GLTexture::operator=(GLTexture&& other) noexcept
{
    if (this != &other)
    {
        Delete();

        ID = other.ID;
        type = other.type;
        internalFormat = other.internalFormat;
        readFormat = other.readFormat;
        pixelType = other.pixelType;
        width = other.width;
        height = other.height;

        other.ResetToDefault();
    }
    return *this;
}

void GLTexture::ResetToDefault()
{
    ID = 0;
    type = TextureType::Texture2D;
    internalFormat = TextureInternalFormat::RGBA8;
    readFormat = TextureFormat::RGBA;
    pixelType = TexturePixelType::UnsignedByte;
    width = 0;
    height = 0;
}

void GLTexture::GenerateMipmap() const
{
    if (ID != 0)
    {
        glBindTexture(ToGL(type), ID);
        glGenerateMipmap(ToGL(type));
        glBindTexture(ToGL(type), 0);
    }
}

TextureID GLTexture::GetID() const{return ID;}
TextureType GLTexture::GetType() const{return type;}
TextureInternalFormat GLTexture::GetInternalFormat() const{return internalFormat;}
TextureFormat GLTexture::GetReadFormat() const{return readFormat;}
TexturePixelType GLTexture::GetPixelType() const{return pixelType;}
uint32_t GLTexture::GetWidth() const{return width;}
uint32_t GLTexture::GetHeight() const{return height;}