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

UTexture::UTexture(std::filesystem::path inSourcePath,
				   const RHI::TextureDesc& inDesc,
				   std::string inName)
	: UObject(std::move(inName))
	, sourcePath(std::move(inSourcePath))
	, desc(inDesc)
{
}

UTexture::~UTexture()
{
	Unload();
}

bool UTexture::Load()
{
	Unload();

	CE_STB::FImageData image = CE_STB::LoadImage(sourcePath);

    if (!image.Pixels)
    {
        CE_LOG(Error, "Failed To Load Texture: [{}]", this->GetName());
        return false;
    }

	desc.width = static_cast<uint32>(image.Width);
	desc.height = static_cast<uint32>(image.Height);
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
			CE_LOG(Warning, "Failed To Load Texture: [{}]", this->GetName());
			CE_STB::FreeImageData(image);
			return false;
	}


	handle = RHI::Get().CreateTexture(desc, image.Pixels);

	if(!handle.IsValid())
	{
		CE_LOG(Error, "Failed To Load Texture: [{}]", this->GetName());
		CE_STB::FreeImageData(image);
		return false;
	}

	CE_STB::FreeImageData(image);

	return true;
}

void UTexture::Unload()
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

void UTexture::Bind(uint32_t slot) const
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