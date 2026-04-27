#pragma once

#include <Project/ProjectDescriptor.h>

#include <optional>
#include <string_view>
#include <string>
#include <vector>

class ProjectRegistry
{
public:
    explicit ProjectRegistry(std::filesystem::path registryPath);

    bool Load();

    const std::vector<ProjectDescriptor>& GetProjects() const;

    std::optional<ProjectDescriptor> FindByName(std::string_view name) const;
    std::optional<ProjectDescriptor> FindByRootPath(const std::filesystem::path& rootPath) const;

private:
    std::filesystem::path registryPath;
    std::vector<ProjectDescriptor> projects;
};