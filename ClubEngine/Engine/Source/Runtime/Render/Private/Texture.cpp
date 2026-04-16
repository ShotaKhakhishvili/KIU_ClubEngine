#include <stb/stb_image.h>
#include <Render/Texture.h>
#include <Core/ClubCore.h>
#include <cassert>

Texture::Texture(
        const uint32_t width, 
        const uint32_t height, 
        const void* data,
        TextureSlot slot,
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType
    )
    :   type(TextureType::Texture2D), 
        slot(slot),    
        internalFormat(internalFormat), 
        readFormat(readFormat), 
        pixelType(pixelType)
{
    SetTextureData(width, height, data);
}

Texture::Texture(
        const std::filesystem::path& imagePath,
        TextureSlot slot,
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType
    )
    :   type(TextureType::Texture2D), 
        slot(slot), 
        internalFormat(internalFormat), 
        readFormat(readFormat), 
        pixelType(pixelType)
{
    ImageData image = LoadImage(imagePath);

    if (!image.Pixels)
    {
        CE_LOG(Error, "Failed to load texture: {}", imagePath);
        return;
    }

    SetTextureData(image.Width, image.Height, image.Pixels);

    stbi_image_free(image.Pixels);
}

Texture::Texture(
        const std::array<std::filesystem::path, 6>& facePaths,
        TextureSlot slot,
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType
    )
    :   type(TextureType::TextureCubeMap),
        slot(slot),
        internalFormat(internalFormat),
        readFormat(readFormat),
        pixelType(pixelType)
{
    std::array<ImageData, 6> faces{};

    for (size_t i = 0; i < 6; ++i)
    {
        faces[i] = LoadImage(facePaths[i], false);

        if (!faces[i].Pixels)
        {
            CE_LOG(Error, "Failed to load cubemap face: {}", facePaths[i]);

            for (size_t j = 0; j < i; ++j)
                stbi_image_free(faces[j].Pixels);

            return;
        }
        
        if(i > 0)
        {
            assert(faces[i].Width == faces[0].Width);
            assert(faces[i].Height == faces[0].Height);
            assert(faces[i].Channels == faces[0].Channels);
        }
    }

    SetCubemapData(faces);

    for (size_t i = 0; i < 6; ++i)
        stbi_image_free(faces[i].Pixels);
}

void Texture::SetTextureData(
        const uint32_t width, 
        const uint32_t height, 
        const void* data
    )
{
    Delete();

    this->width = width;
    this->height = height;

    assert(type == TextureType::Texture2D);

    glGenTextures(1, &ID);
    glActiveTexture(static_cast<GLenum>(slot));
    glBindTexture(static_cast<GLenum>(type), ID);

    glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(static_cast<GLenum>(type), GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        static_cast<GLenum>(type),
        0,
        static_cast<GLenum>(internalFormat),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0,
        static_cast<GLenum>(readFormat),
        static_cast<GLenum>(pixelType),
        data
    );

    GenerateMipmap();
}

void Texture::SetCubemapData(const std::array<ImageData, 6>& faces)
{
    Delete();

    assert(type == TextureType::TextureCubeMap);

    width = static_cast<uint32_t>(faces[0].Width);
    height = static_cast<uint32_t>(faces[0].Height);

    for (size_t i = 1; i < 6; ++i)
    {
        assert(faces[i].Width == faces[0].Width);
        assert(faces[i].Height == faces[0].Height);
    }

    glGenTextures(1, &ID);
    glActiveTexture(static_cast<GLenum>(slot));
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    for (size_t i = 0; i < 6; ++i)
    {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            static_cast<GLenum>(internalFormat),
            static_cast<GLsizei>(faces[i].Width),
            static_cast<GLsizei>(faces[i].Height),
            0,
            static_cast<GLenum>(readFormat),
            static_cast<GLenum>(pixelType),
            faces[i].Pixels
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    GenerateMipmap();
}

void Texture::TexUnit(Shader& shader, const char* uniform) const
{
    shader.Bind();
    const GLint unit = static_cast<GLint>(static_cast<GLuint>(slot) - GL_TEXTURE0);
    const GLint location = glGetUniformLocation(shader.GetID(), uniform);

    if(location != -1)
        glUniform1i(location, unit);
}

void Texture::Bind() const
{
    assert(ID != 0);
    glBindTexture(static_cast<GLenum>(type), ID);
}

void Texture::Unbind() const
{
    glBindTexture(static_cast<GLenum>(type), 0);
}

void Texture::Delete()
{
    if (ID != 0)
    {
        glDeleteTextures(1, &ID);
        ID = 0;
    }
}

Texture::~Texture()
{
    Delete();
}

Texture::Texture(Texture&& other) noexcept
    :   ID(other.ID), 
        type(other.type), 
        slot(other.slot), 
        internalFormat(other.internalFormat),
        readFormat(other.readFormat), 
        pixelType(other.pixelType), 
        width(other.width), 
        height(other.height)
{
    other.ResetToDefault();
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        Delete();

        ID = other.ID;
        type = other.type;
        slot = other.slot;
        internalFormat = other.internalFormat;
        readFormat = other.readFormat;
        pixelType = other.pixelType;
        width = other.width;
        height = other.height;

        other.ResetToDefault();
    }
    return *this;
}

void Texture::ResetToDefault()
{
    ID = 0;
    type = TextureType::Texture2D;
    slot = TextureSlot::Slot0;
    internalFormat = TextureInternalFormat::RGBA8;
    readFormat = TextureFormat::RGBA;
    pixelType = TexturePixelType::UnsignedByte;
    width = 0;
    height = 0;
}

void Texture::GenerateMipmap() const
{
    if (ID != 0)
    {
        glBindTexture(static_cast<GLenum>(type), ID);
        glGenerateMipmap(static_cast<GLenum>(type));
        glBindTexture(static_cast<GLenum>(type), 0);
    }
}

TextureID Texture::GetID() const{return ID;}
TextureType Texture::GetType() const{return type;}
TextureSlot Texture::GetSlot() const{return slot;}
TextureInternalFormat Texture::GetInternalFormat() const{return internalFormat;}
TextureFormat Texture::GetReadFormat() const{return readFormat;}
TexturePixelType Texture::GetPixelType() const{return pixelType;}
uint32_t Texture::GetWidth() const{return width;}
uint32_t Texture::GetHeight() const{return height;}