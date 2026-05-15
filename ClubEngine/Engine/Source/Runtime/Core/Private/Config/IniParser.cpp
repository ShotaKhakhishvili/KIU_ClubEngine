#include <Core/Config/IniParser.h>
#include <Core/Log.h>

#include <fstream>
#include <sstream>

namespace CE
{

bool IniParser::Parse(const std::filesystem::path& path, IniData& outData)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        CE_LOG(Warning, "IniParser: Could not open {}", path.string());
        return false;
    }

    std::string currentSection;
    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        if (line[0] == '[')
        {
            auto end = line.find(']');
            if (end != std::string::npos)
                currentSection = line.substr(1, end - 1);
            continue;
        }

        auto eq = line.find('=');
        if (eq != std::string::npos && !currentSection.empty())
        {
            std::string key   = line.substr(0, eq);
            std::string value = line.substr(eq + 1);
            outData[currentSection][key] = value;
        }
    }

    return true;
}

}