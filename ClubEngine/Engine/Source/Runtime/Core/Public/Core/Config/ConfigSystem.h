#pragma once

#include <Core/Config/IniTypes.h>

#include <filesystem>
#include <string>

namespace CE
{

class ConfigSystem
{
public:
    void LoadFile(const std::filesystem::path& path);

    template<typename T>
    T Get(const std::string& section, const std::string& key, const T& defaultValue) const;

private:
    IniData data;
};

template<>
std::string ConfigSystem::Get(const std::string& section, const std::string& key, const std::string& defaultValue) const;

template<>
int ConfigSystem::Get(const std::string& section, const std::string& key, const int& defaultValue) const;

template<>
float ConfigSystem::Get(const std::string& section, const std::string& key, const float& defaultValue) const;

template<>
bool ConfigSystem::Get(const std::string& section, const std::string& key, const bool& defaultValue) const;

}