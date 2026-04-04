#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <cstddef>

namespace CE::FileIO
{
    /**
     * @brief Reads an entire text file into a string.
     * @param path Path to the file.
     * @return File contents as std::string.
     */
    std::string ReadTextFile(const std::filesystem::path& path);
}