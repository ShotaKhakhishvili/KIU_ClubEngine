#pragma once

#include <AssetImport/FMeshImportResult.h>

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace CE
{
    struct FObjImportSettings
    {
        bool flipV = true;
        bool triangulate = true;
        float scale = 1.0f;
        bool convertAxes = true;
    };

    class ObjMeshImporter
    {
    public:
        explicit ObjMeshImporter(FObjImportSettings settings = {});

        std::optional<FMeshImportResult> Import(const std::filesystem::path& path);

        const std::vector<std::string>& GetErrors() const;
        const std::vector<std::string>& GetWarnings() const;

    private:
        void ClearMessages();

        void AddError(std::string message);
        void AddWarning(std::string message);

        FObjImportSettings settings;

        std::vector<std::string> errors;
        std::vector<std::string> warnings;

    };
}