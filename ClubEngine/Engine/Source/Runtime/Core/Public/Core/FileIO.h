#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <cstddef>


namespace CE::FileIO
{
    std::string ReadFile(const std::filesystem::path& path);
    std::string ReadFileWithIncludes(const std::filesystem::path& path);
}