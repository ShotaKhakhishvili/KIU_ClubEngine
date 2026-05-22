#include <Core/FileIO.h>

#include <fstream>
#include <sstream>

namespace CE::FileIO
{

static std::filesystem::path ParseIncludePath(const std::string& line)
{
    auto start = line.find('"');
    auto end   = line.rfind('"');

    if (start == std::string::npos || start == end)
        return {};

    return line.substr(start + 1, end - start - 1);
}

std::string ReadFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Failed to open file: " + path.string());

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string ReadFileWithIncludes(const std::filesystem::path& path)
{
    std::ifstream file(path);

    std::ostringstream result;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.starts_with("#include"))
        {
            auto includePath = ParseIncludePath(line);

            auto resolvedPath = path.parent_path() / includePath;

            result << ReadFileWithIncludes(resolvedPath);
        }
        else
        {
            result << line << "\n";
        }
    }

    return result.str();
}

}