#include "GLConvert.h"

#include <Core/ClubCore.h>

#include <RHI.OpenGL/GLTexture.h>

#include <cassert>

namespace CE::RHI
{

GLTexture::GLTexture(const TextureDesc& desc, const void* data)
    :   type(TextureType::Texture2D),
        internalFormat(desc.format),
        readFormat(desc.readFormat),
        pixelType(desc.pixelType),
        width(desc.width),
        height(desc.height),
        generateMipmaps(desc.generateMipmaps)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLMinFilter(desc.minFilter, desc.generateMipmaps));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLMagFilter(desc.magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGL(desc.wrapU));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGL(desc.wrapV));

    // Decoded images are tightly packed, RGB rows with odd widths break the default 4-byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        ToGLInternalFormat(internalFormat, desc.sRGB),
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

GLTexture::GLTexture(const TextureDesc& desc, const void* const* faceData)
    :   type(TextureType::TextureCubeMap),
        internalFormat(desc.format),
        readFormat(desc.readFormat),
        pixelType(desc.pixelType),
        width(desc.width),
        height(desc.height),
        generateMipmaps(desc.generateMipmaps)
{
    assert(faceData != nullptr);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (size_t i = 0; i < 6; ++i)
    {
        assert(faceData[i] != nullptr);

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i),
            0,
            ToGLInternalFormat(internalFormat, desc.sRGB),
            static_cast<GLsizei>(width),
            static_cast<GLsizei>(height),
            0,
            ToGL(readFormat),
            ToGL(pixelType),
            faceData[i]
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, ToGLMinFilter(desc.minFilter, desc.generateMipmaps));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, ToGLMagFilter(desc.magFilter));

    // Cubemaps always clamp, repeating would show seams between the faces
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

}