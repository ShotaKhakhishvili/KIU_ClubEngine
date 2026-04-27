#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <AssetImport/StaticMeshImportData.h>

struct ObjImportSettings
{
    bool flipV = true;
    bool triangulate = true;
    float scale = 1.0f;
};

class ObjMeshImporter
{
public:
    explicit ObjMeshImporter(ObjImportSettings settings = {});

    std::optional<StaticMeshImportData> Import(const std::filesystem::path& path);

    const std::vector<std::string>& GetErrors() const;
    const std::vector<std::string>& GetWarnings() const;

private:
    ObjImportSettings settings;

    std::vector<std::string> errors;
    std::vector<std::string> warnings;

private:
    void ClearMessages();

    void AddError(std::string message);
    void AddWarning(std::string message);
};