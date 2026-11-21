#pragma once

#include <Core/Types/RHITypes.h>

#include <CoreUObject/UObject.h>

#include <RenderCore/RHI/RHIHandle.h>

#include <filesystem>
#include <memory>
#include <string>
#include <cstdint>

namespace CE_STB
{
struct FImageData;
}
namespace CE
{

class UTexture final : public UObject
{
    GENERATED_BODY(UTexture, UObject)

public:
    UTexture() = default;
    ~UTexture() override;

    bool BuildFromSourceFile(const std::filesystem::path& path, const RHI::TextureDesc& newDesc);
    bool BuildFromData(const TArray<uint8>& encodedBytes, const RHI::TextureDesc& newDesc);

    bool IsReady() const noexcept;

    void Bind(uint32_t slot) const;

    uint32_t GetWidth() const noexcept;
    uint32_t GetHeight() const noexcept;

private:
    bool UploadImage(const CE_STB::FImageData& image, const RHI::TextureDesc& newDesc);
    void Release();

    RHI::TextureDesc desc;
    RHI::TextureHandle handle;    
};

}