#pragma once

#include <filesystem>
#include <string>

namespace CE
{
struct FProjectDescriptor
{
    std::string name;
    std::filesystem::path rootPath;

    std::filesystem::path GetEnginePath() const;
    std::filesystem::path GetEngineConfigPath() const;

    std::filesystem::path GetProjectPath() const;

    std::filesystem::path GetContentPath() const;
    std::filesystem::path GetConfigPath() const;

    std::filesystem::path ResolveContentPath(const std::filesystem::path& relativePath) const;
    std::filesystem::path ResolveAssetPath(const std::filesystem::path& path) const;
};

}
