#include <Core/ClubCore.h>

#include <Project/ProjectRegistry.h>

#include <algorithm>
#include <fstream>
#include <sstream>

ProjectRegistry::ProjectRegistry(std::filesystem::path inRegistryPath)
    : registryPath(std::move(inRegistryPath))
{
}

bool ProjectRegistry::Load()
{
    projects.clear();

    std::ifstream file(registryPath);

    if (!file.is_open())
    {
        CE_LOG(Warning, "Failed to open project registry: {}", registryPath.string());
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    const std::string text = buffer.str();

    size_t pos = 0;

    while (true)
    {
        const size_t nameKey = text.find("\"name\"", pos);

        if (nameKey == std::string::npos)
        {
            break;
        }

        const size_t nameColon = text.find(':', nameKey);
        const size_t nameStart = text.find('"', nameColon + 1);
        const size_t nameEnd = text.find('"', nameStart + 1);

        const size_t rootKey = text.find("\"rootPath\"", nameEnd);
        const size_t rootColon = text.find(':', rootKey);
        const size_t rootStart = text.find('"', rootColon + 1);
        const size_t rootEnd = text.find('"', rootStart + 1);

        if (nameColon == std::string::npos ||
            nameStart == std::string::npos ||
            nameEnd == std::string::npos ||
            rootKey == std::string::npos ||
            rootColon == std::string::npos ||
            rootStart == std::string::npos ||
            rootEnd == std::string::npos)
        {
            CE_LOG(Warning, "Malformed project registry: {}", registryPath.string());
            return false;
        }

        ProjectDescriptor descriptor;
        descriptor.name = text.substr(nameStart + 1, nameEnd - nameStart - 1);
        descriptor.rootPath = text.substr(rootStart + 1, rootEnd - rootStart - 1);

        projects.push_back(std::move(descriptor));

        pos = rootEnd + 1;
    }

    return true;
}

const std::vector<ProjectDescriptor>& ProjectRegistry::GetProjects() const
{
    return projects;
}

std::optional<ProjectDescriptor> ProjectRegistry::FindByName(std::string_view name) const
{
    auto it = std::find_if(projects.begin(), projects.end(),
        [&](const ProjectDescriptor& project)
        {
            return project.name == name;
        });

    if (it == projects.end())
    {
        return std::nullopt;
    }

    return *it;
}

std::optional<ProjectDescriptor> ProjectRegistry::FindByRootPath(const std::filesystem::path& rootPath) const
{
    const auto normalizedRoot = std::filesystem::weakly_canonical(rootPath);

    auto it = std::find_if(projects.begin(), projects.end(),
        [&](const ProjectDescriptor& project)
        {
            return std::filesystem::weakly_canonical(project.rootPath) == normalizedRoot;
        });

    if (it == projects.end())
    {
        return std::nullopt;
    }

    return *it;
}