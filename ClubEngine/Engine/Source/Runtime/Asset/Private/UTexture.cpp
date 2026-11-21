#include <Core/ClubCore.h>

#include <RenderCore/RHI/RHIHandle.h>
#include <RenderCore/RHI/IRHI.h>

#include <Asset/UTexture.h>

#include <AssetImport/Image.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

namespace CE
{

IMPLEMENT_CLASS(UTexture)

UTexture::~UTexture()
{
	Release();
}

bool UTexture::BuildFromSourceFile(const std::filesystem::path& path, const RHI::TextureDesc& newDesc)
{
    CE_STB::FImageData image = CE_STB::LoadImage(path);

    if (image.Pixels == nullptr)
    {
        CE_LOG(Error, "Failed To Load Texture File: [{}]", path.string());
        return false;
    }

    const bool uploaded = UploadImage(image, newDesc);

    CE_STB::FreeImageData(image);
    return uploaded;
}

bool UTexture::BuildFromData(const TArray<uint8>& encodedBytes, const RHI::TextureDesc& newDesc)
{
    CE_STB::FImageData image = CE_STB::LoadImageFromMemory(encodedBytes.data(), encodedBytes.size());

    if (image.Pixels == nullptr)
    {
        CE_LOG(Error, "Failed To Decode Texture Data: [{}]", this->GetName());
        return false;
    }

    const bool uploaded = UploadImage(image, newDesc);

    CE_STB::FreeImageData(image);
    return uploaded;
}

bool UTexture::UploadImage(const CE_STB::FImageData& image, const RHI::TextureDesc& newDesc)
{
    Release();

    desc = newDesc;
    desc.width = image.Width;
    desc.height = image.Height;
    desc.pixelType = RHI::TexturePixelType::UnsignedByte;

    switch (image.Channels)
    {
        case 1:
            desc.format = RHI::TextureInternalFormat::R8;
            desc.readFormat = RHI::TextureFormat::Red;
            break;
        case 2:
            desc.format = RHI::TextureInternalFormat::RG8;
            desc.readFormat = RHI::TextureFormat::RG;
            break;
        case 3:
            desc.format = RHI::TextureInternalFormat::RGB8;
            desc.readFormat = RHI::TextureFormat::RGB;
            break;
        case 4:
            desc.format = RHI::TextureInternalFormat::RGBA8;
            desc.readFormat = RHI::TextureFormat::RGBA;
            break;
        default:
            CE_LOG(Error, "Unsupported Texture Channel Count {}: [{}]", image.Channels, this->GetName());
            return false;
    }

    handle = RHI::Get().CreateTexture(desc, image.Pixels);

    if (!handle.IsValid())
    {
        CE_LOG(Error, "Failed To Create Texture: [{}]", this->GetName());
        return false;
    }

    return true;
}

void UTexture::Release()
{
	if(!handle.IsValid())
		return;

	RHI::Get().DestroyTexture(handle);
	handle.Invalidate();
}

bool UTexture::IsReady() const noexcept
{
	return handle.IsValid();
}

void UTexture::Bind(const uint32_t slot) const
{
	if(!handle.IsValid())
		return;

	RHI::Get().BindTexture(handle, slot);
}

uint32_t UTexture::GetWidth() const noexcept
{
	if(!handle.IsValid())
		return 0;

	return desc.width;
}

uint32_t UTexture::GetHeight() const noexcept
{
	if(!handle.IsValid())
		return 0;

	return desc.height;
}

}