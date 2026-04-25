#define STB_IMAGE_IMPLEMENTATION

#include <AssetImport/Image.h>

#include <stb/stb_image.h>

namespace CE_STB
{
    ImageData LoadImage(const std::filesystem::path& path, bool flipVertically)
    {
        ImageData image;
        stbi_set_flip_vertically_on_load(flipVertically);
        image.Pixels = stbi_load(path.string().c_str(), &image.Width, &image.Height, &image.Channels, 0);
        return image;
    }

    void FreeImageData(ImageData& image)
    {
        stbi_image_free(image.Pixels);
    }
}