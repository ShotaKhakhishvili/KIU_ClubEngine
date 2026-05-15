#pragma once

#include <string>
#include <unordered_map>

namespace CE
{

// section → key → value
using IniData = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

}