#pragma once

#include <Render/RenderTypes.h>
#include <Render/Texture/Image.h>
#include <Render/Shader.h>
#include <filesystem>
#include <array>

class Texture
{
public:
    Texture(
        const std::filesystem::path& imagePath,
        TextureSlot slot, 
        TextureInternalFormat internalFormat, 
        TextureFormat readFormat, 
        TexturePixelType pixelType
    );
    Texture(
        const uint32_t width, 
        const uint32_t height, 
        const void* data,
        TextureSlot slot,
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType
    );
    Texture(
        const std::array<std::filesystem::path, 6>& facePaths,
        TextureSlot slot,
        TextureInternalFormat internalFormat,
        TextureFormat readFormat,
        TexturePixelType pixelType
    );
        
    ~Texture();

    void TexUnit(Shader& shader, const char* uniform) const;
    void GenerateMipmap() const;
    void Bind() const;
    void Unbind() const;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    TextureID GetID() const;
    TextureType GetType() const;
    TextureSlot GetSlot() const;
    TextureInternalFormat GetInternalFormat() const;
    TextureFormat GetReadFormat() const;
    TexturePixelType GetPixelType() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:
    TextureID ID{0};

    TextureType type{TextureType::Texture2D};
    TextureSlot slot{TextureSlot::Slot0};
    
    TextureInternalFormat internalFormat{TextureInternalFormat::RGBA8};
    TextureFormat readFormat{TextureFormat::RGBA};
    TexturePixelType pixelType{TexturePixelType::UnsignedByte};

    uint32_t width{0};
    uint32_t height{0};

    void ResetToDefault();
    void Delete();
    void SetTextureData(uint32_t width, uint32_t height, const void* data);
    void SetCubemapData(const std::array<ImageData, 6>& faces);
};