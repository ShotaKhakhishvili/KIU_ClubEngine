#include <Core/ClubCore.h>

#include <Asset/UTexture.h>

#include <AssetImport/Image.h>

#include <RHI.OpenGL/Texture.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

UTexture::UTexture(std::filesystem::path inSourcePath,
				   ImportSettings inSettings,
				   std::string inName)
	: UObject(std::move(inName))
	, sourcePath(std::move(inSourcePath))
	, settings(inSettings)
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
        CE_LOG(Error, "Failed to load texture: {}", sourcePath.string());
        return false;
    }

	resource = std::make_unique<Texture>(
		image.Width,
		image.Height,
		settings.internalFormat,
		settings.readFormat,
		settings.pixelType,
		settings.generateMipmaps,
		image.Pixels
	);

	if(resource == nullptr || resource->GetID() == 0)
	{
		resource.reset();
		return false;
	}

	CE_STB::FreeImageData(image);

	return true;
}

void UTexture::Unload()
{
	resource.reset();
}

bool UTexture::IsReady() const noexcept
{
	return resource != nullptr && resource->GetID() != 0;
}

void UTexture::Bind(uint32_t slot) const
{
	if(resource == nullptr)
		return;

	resource->Bind(static_cast<int32_t>(slot));
}

uint32_t UTexture::GetWidth() const noexcept
{
	if(resource == nullptr)
		return 0;

	return resource->GetWidth();
}

uint32_t UTexture::GetHeight() const noexcept
{
	if(resource == nullptr)
		return 0;

	return resource->GetHeight();
}