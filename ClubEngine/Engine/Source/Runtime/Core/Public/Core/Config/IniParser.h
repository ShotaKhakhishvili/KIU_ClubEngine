#pragma once

#include <Core/Config/IniTypes.h>

#include <filesystem>

namespace CE
{

class IniParser
{
public:
    static bool Parse(const std::filesystem::path& path, IniData& out);
};

}