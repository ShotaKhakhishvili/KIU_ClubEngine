#pragma once

#include <filesystem>
#include <string>

struct ProjectDescriptor
{
    std::string name;
    std::filesystem::path rootPath;

    std::filesystem::path GetContentPath() const
    {
        return rootPath / "Content";
    }

    std::filesystem::path ResolveContentPath(const std::filesystem::path& relativePath) const
    {
        return GetContentPath() / relativePath;
    }

    std::filesystem::path ResolveAssetPath(const std::filesystem::path& path) const
    {
        const std::string text = path.generic_string();

        if (text.rfind("/Game/", 0) == 0)
        {
            return GetContentPath() / text.substr(6);
        }

        if (path.is_absolute())
        {
            return path;
        }

        return GetContentPath() / path;
    }
};