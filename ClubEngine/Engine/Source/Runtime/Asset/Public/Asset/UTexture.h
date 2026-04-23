#pragma once

#include <CoreUObject/UObject.h>

#include <RenderCore/RenderTypes.h>

#include <filesystem>
#include <memory>
#include <string>
#include <cstdint>

class Texture;

class UTexture final : public UObject
{
public:
    struct ImportSettings
    {
        ImportSettings(
            TextureSlot inSlot = TextureSlot::Slot0,
            TextureInternalFormat inInternalFormat = TextureInternalFormat::RGBA8,
            TextureFormat inReadFormat = TextureFormat::RGBA,
            TexturePixelType inPixelType = TexturePixelType::UnsignedByte)
            : slot(inSlot)
            , internalFormat(inInternalFormat)
            , readFormat(inReadFormat)
            , pixelType(inPixelType)
        {
        }

        TextureSlot slot;
        TextureInternalFormat internalFormat;
        TextureFormat readFormat;
        TexturePixelType pixelType;
    };

    UTexture(std::filesystem::path inSourcePath,
             ImportSettings inSettings = ImportSettings(),
             std::string inName = "UTexture");
    ~UTexture() override;

    UTexture(const UTexture&) = delete;
    UTexture& operator=(const UTexture&) = delete;
    UTexture(UTexture&&) noexcept = default;
    UTexture& operator=(UTexture&&) noexcept = default;

    const char* GetClassName() const noexcept override;

    bool Load();
    void Unload();
    bool IsReady() const noexcept;

    void Bind(uint32_t slot) const;

    uint32_t GetWidth() const noexcept;
    uint32_t GetHeight() const noexcept;

private:
    std::filesystem::path sourcePath;
    ImportSettings settings;
    std::unique_ptr<Texture> resource;    
};
