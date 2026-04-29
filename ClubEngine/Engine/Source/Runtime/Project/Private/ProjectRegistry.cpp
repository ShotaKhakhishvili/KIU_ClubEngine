#include <Core/ClubCore.h>

#include <Project/ProjectRegistry.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

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

    const auto j = nlohmann::json::parse(file, nullptr, false);
    if (j.is_discarded())
    {
        CE_LOG(Warning, "Malformed project registry: {}", registryPath.string());
        return false;
    }

    for (const auto& entry : j)
    {
        ProjectDescriptor descriptor;
        descriptor.name = entry["name"].get<std::string>();
        descriptor.rootPath = entry["rootPath"].get<std::string>();
        projects.push_back(std::move(descriptor));
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