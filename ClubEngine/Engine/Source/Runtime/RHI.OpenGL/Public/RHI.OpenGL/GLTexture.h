#pragma once

#include <Core/Types/RHITypes.h>

#include <filesystem>
#include <array>

namespace CE::RHI
{

class GLTexture
{
public:
    GLTexture(const TextureDesc& desc, const void* data);
    GLTexture(const TextureDesc& desc, const void* const* faceData);
        
    ~GLTexture();

    void GenerateMipmap() const;
    void Bind(int32_t slot) const;
    void Unbind() const;

    GLTexture(const GLTexture&) = delete;
    GLTexture& operator=(const GLTexture&) = delete;

    GLTexture(GLTexture&& other) noexcept;
    GLTexture& operator=(GLTexture&& other) noexcept;

    TextureID GetID() const;
    TextureType GetType() const;
    TextureInternalFormat GetInternalFormat() const;
    TextureFormat GetReadFormat() const;
    TexturePixelType GetPixelType() const;
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:
    TextureID ID{0};

    TextureType type;
    
    TextureInternalFormat internalFormat{TextureInternalFormat::RGBA8};
    TextureFormat readFormat{TextureFormat::RGBA};
    TexturePixelType pixelType{TexturePixelType::UnsignedByte};

    uint32_t width{0};
    uint32_t height{0};

    bool generateMipmaps;

    void ResetToDefault();
    void Delete();
};

}