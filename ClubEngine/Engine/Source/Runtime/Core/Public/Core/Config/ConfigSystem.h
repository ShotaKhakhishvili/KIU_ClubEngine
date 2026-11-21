#pragma once

#include <Core/Containers/TArray.h>

#include <Core/Config/IniTypes.h>

#include <filesystem>
#include <string>

namespace CE
{

class ConfigSystem
{
public:
    void LoadFile(const std::filesystem::path& path);
    void AddCommandLine(const TArray<std::string>& commandLine);

    template<typename T>
    T Get(const std::string& section, const std::string& key) const;

private:
    IniData data;
};

}

#include "ConfigSystem.inl"