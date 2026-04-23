#pragma once

#include <filesystem>

struct ImageData
{
    unsigned char* Pixels = nullptr;
    int Width = 0;
    int Height = 0;
    int Channels = 0;
};

ImageData LoadImage(const std::filesystem::path& path, bool flipVertically = true);