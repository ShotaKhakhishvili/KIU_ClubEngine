#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <AssetImport/Image.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

namespace CE_STB
{
    FImageData LoadImage(const std::filesystem::path& path, bool flipVertically)
    {
        FImageData image;
        stbi_set_flip_vertically_on_load(flipVertically);
        image.Pixels = stbi_load(path.string().c_str(), &image.Width, &image.Height, &image.Channels, 0);
        return image;
    }

    bool SavePNG(const std::filesystem::path& path, const unsigned char* pixels, int width, int height, int channels)
    {
        return stbi_write_png(path.string().c_str(), width, height, channels, pixels, width * channels) != 0;
    }

    void FreeImageData(FImageData& image)
    {
        stbi_image_free(image.Pixels);
    }
}