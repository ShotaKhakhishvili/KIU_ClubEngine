#include <Core/ClubCore.h>

#include <RenderCore/RHI/RHIHandle.h>
#include <RenderCore/RHI/IRHI.h>

#include <Asset/UTexture.h>

#include <AssetImport/Image.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

UTexture::UTexture(std::filesystem::path inSourcePath,
				   const TextureDesc& inDesc,
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

const char* UTexture::GetClassName() const noexcept
{
	return "UTexture";
}

bool UTexture::Load()
{
	Unload();

	CE_STB::ImageData image = CE_STB::LoadImage(sourcePath);

    if (!image.Pixels)
    {
        CE_LOG(Error, "Failed To Load Texture: {}", this->GetName());
        return false;
    }

	handle = RHI::Get().CreateTexture(desc, image.Pixels);

	if(!handle.IsValid())
	{
		CE_LOG(Error, "Failed To Load Texture: {}", this->GetName());
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