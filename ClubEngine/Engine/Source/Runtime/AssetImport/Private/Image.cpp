#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <AssetImport/Image.h>

#include <Core/Types/CoreTypes.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <limits>

namespace CE_STB
{

namespace
{

FImageData MakeImageData(unsigned char* pixels, const int width, const int height, const int channels)
{
    FImageData image;

    if (pixels == nullptr)
        return image;

    image.Pixels = pixels;
    image.Width = static_cast<CE::uint32>(width);
    image.Height = static_cast<CE::uint32>(height);
    image.Channels = static_cast<CE::uint32>(channels);

    return image;
}

}

FImageData LoadImage(const std::filesystem::path& path, const bool flipVertically)
{
    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char* pixels = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    return MakeImageData(pixels, width, height, channels);
}

FImageData LoadImageFromMemory(const unsigned char* data, const size_t size, const bool flipVertically)
{
    if (data == nullptr || size == 0 || size > static_cast<size_t>(std::numeric_limits<int>::max()))
        return {};

    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char* pixels = stbi_load_from_memory(data, static_cast<int>(size), &width, &height, &channels, 0);

    return MakeImageData(pixels, width, height, channels);
}

bool SavePNG(const std::filesystem::path& path, const unsigned char* pixels, const int width, const int height, const int channels)
{
    return stbi_write_png(path.string().c_str(), width, height, channels, pixels, width * channels) != 0;
}

void FreeImageData(FImageData& image)
{
    stbi_image_free(image.Pixels);
    image = {};
}

}