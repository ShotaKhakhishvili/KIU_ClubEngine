#include <Core/Config/ConfigSystem.h>
#include <Core/Config/IniParser.h>
#include <Core/Log.h>

namespace CE
{

void ConfigSystem::LoadFile(const std::filesystem::path& path)
{
    IniParser::Parse(path, data);
}

template<>
std::string ConfigSystem::Get(const std::string& section, const std::string& key, const std::string& defaultValue) const
{
    auto secIt = data.find(section);
    if (secIt == data.end()) return defaultValue;

    auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end()) return defaultValue;

    return keyIt->second;
}

template<>
int ConfigSystem::Get(const std::string& section, const std::string& key, const int& defaultValue) const
{
    auto str = Get<std::string>(section, key, "");
    if (str.empty()) return defaultValue;
    try { return std::stoi(str); }
    catch (...) { return defaultValue; }
}

template<>
float ConfigSystem::Get(const std::string& section, const std::string& key, const float& defaultValue) const
{
    auto str = Get<std::string>(section, key, "");
    if (str.empty()) return defaultValue;
    try { return std::stof(str); }
    catch (...) { return defaultValue; }
}

template<>
bool ConfigSystem::Get(const std::string& section, const std::string& key, const bool& defaultValue) const
{
    auto str = Get<std::string>(section, key, "");
    if (str.empty()) return defaultValue;
    return str == "true" || str == "1" || str == "True";
}

}