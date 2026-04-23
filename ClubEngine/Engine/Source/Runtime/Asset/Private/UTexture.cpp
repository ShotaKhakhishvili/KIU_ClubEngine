#include <Asset/UTexture.h>

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

	resource = std::make_unique<Texture>(
		sourcePath,
		settings.slot,
		settings.internalFormat,
		settings.readFormat,
		settings.pixelType
	);

	if(resource == nullptr || resource->GetID() == 0)
	{
		resource.reset();
		return false;
	}

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