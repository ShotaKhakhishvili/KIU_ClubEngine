#include "FileIO.h"

#include <fstream>
#include <sstream>

namespace CE::FileIO
{
    std::string ReadTextFile(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
            throw std::runtime_error("Failed to open file: " + path.string());

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    void WriteTextFile(const std::filesystem::path& path, std::string_view content)
    {
    }

    void AppendTextFile(const std::filesystem::path& path, std::string_view content)
    {
    }

    std::vector<std::byte> ReadBinaryFile(const std::filesystem::path& path)
    {
        return {};
    }

    void WriteBinaryFile(const std::filesystem::path& path, const std::vector<std::byte>& data)
    {
    }

    bool Exists(const std::filesystem::path& path)
    {
        return false;
    }

    bool IsDirectory(const std::filesystem::path& path)
    {
        return false;
    }

    bool CreateDirectories(const std::filesystem::path& path)
    {
        return false;
    }

    bool Remove(const std::filesystem::path& path)
    {
        return false;
    }

    std::uintmax_t FileSize(const std::filesystem::path& path)
    {
        return 0;
    }
}