#include <Project/FProjectDescriptor.h>

namespace CE
{

std::filesystem::path FProjectDescriptor::GetEnginePath() const
{
    return std::filesystem::path(CE_ENGINE_ROOT);
}

std::filesystem::path FProjectDescriptor::GetEngineConfigPath() const
{
    return GetEnginePath() / "Engine" / "Config";
}

std::filesystem::path FProjectDescriptor::GetProjectPath() const
{
    return rootPath;
}

std::filesystem::path FProjectDescriptor::GetContentPath() const
{
    return rootPath / "Content";
}

std::filesystem::path FProjectDescriptor::GetConfigPath() const
{
    return rootPath / "Config";
}

std::filesystem::path FProjectDescriptor::ResolveContentPath(const std::filesystem::path& relativePath) const
{
    return GetContentPath() / relativePath;
}

std::filesystem::path FProjectDescriptor::ResolveAssetPath(const std::filesystem::path& path) const
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

}