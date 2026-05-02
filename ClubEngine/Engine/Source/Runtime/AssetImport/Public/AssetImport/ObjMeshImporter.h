#pragma once

#include <AssetImport/MeshImportResult.h>

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace CE
{
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

        std::optional<MeshImportResult> Import(const std::filesystem::path& path);

        const std::vector<std::string>& GetErrors() const;
        const std::vector<std::string>& GetWarnings() const;

    private:
        void ClearMessages();

        void AddError(std::string message);
        void AddWarning(std::string message);

        ObjImportSettings settings;

        std::vector<std::string> errors;
        std::vector<std::string> warnings;

    };
}