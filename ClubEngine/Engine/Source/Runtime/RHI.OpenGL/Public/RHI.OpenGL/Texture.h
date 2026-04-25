#pragma once

#include <RenderCore/RenderTypes.h>

#include <filesystem>
#include <array>

class Texture
{
public:
    Texture(
        const uint32_t width, 
        const uint32_t height, 
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType,
        const void* data
    );
    Texture(
        const uint32_t width,
        const uint32_t height,
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType,
        const void* const* data
    );
        
    ~Texture();

    void GenerateMipmap() const;
    void Bind(int32_t slot) const;
    void Unbind() const;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    TextureID GetID() const;
    TextureType GetType() const;
    TextureInternalFormat GetInternalFormat() const;
    TextureFormat GetReadFormat() const;
    TexturePixelType GetPixelType() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:
    TextureID ID{0};

    TextureType type{TextureType::Texture2D};
    
    TextureInternalFormat internalFormat{TextureInternalFormat::RGBA8};
    TextureFormat readFormat{TextureFormat::RGBA};
    TexturePixelType pixelType{TexturePixelType::UnsignedByte};

    uint32_t width{0};
    uint32_t height{0};

    void ResetToDefault();
    void Delete();
};