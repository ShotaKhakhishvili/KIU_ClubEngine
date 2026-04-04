#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>
#include <filesystem>

struct ImageData
{
    unsigned char* Pixels = nullptr;
    int Width = 0;
    int Height = 0;
    int Channels = 0;
};

static ImageData LoadImage(const std::filesystem::path& path, bool flipVertically = true)
{
    ImageData image;
    stbi_set_flip_vertically_on_load(flipVertically);
    image.Pixels = stbi_load(path.string().c_str(), &image.Width, &image.Height, &image.Channels, 0);
    return image;
}