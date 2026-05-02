#pragma once

#include <Core/Types/RHITypes.h>

#include <CoreUObject/UObject.h>

#include <RenderCore/RHI/RHIHandle.h>

#include <filesystem>
#include <memory>
#include <string>
#include <cstdint>

namespace CE
{

class UTexture final : public UObject
{
public:
    UTexture(std::filesystem::path inSourcePath,
             const RHI::TextureDesc& inDesc,
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
    RHI::TextureDesc desc;
    RHI::TextureHandle handle;    
};

}