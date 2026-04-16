#include <Core/FileIO.h>

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
}