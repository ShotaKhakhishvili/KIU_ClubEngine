#pragma once

#include <filesystem>

namespace CE_STB
{
    struct FImageData
    {
        unsigned char* Pixels = nullptr;
        int Width = 0;
        int Height = 0;
        int Channels = 0;
    };

    FImageData LoadImage(const std::filesystem::path& path, bool flipVertically = true);
    void FreeImageData(FImageData& image);
}