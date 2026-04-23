#define STB_IMAGE_IMPLEMENTATION

#include <RHI.OpenGL/Texture/Image.h>

#include <stb/stb_image.h>

ImageData LoadImage(const std::filesystem::path& path, bool flipVertically)
{
    ImageData image;
    stbi_set_flip_vertically_on_load(flipVertically);
    image.Pixels = stbi_load(path.string().c_str(), &image.Width, &image.Height, &image.Channels, 0);
    return image;
}