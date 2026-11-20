#pragma once

#include <Core/ClubCore.h>

#include <AssetImport/FSceneImportResult.h>

#include <filesystem>
#include <optional>
#include <string>

struct cgltf_data;
struct cgltf_image;
struct cgltf_primitive;

namespace CE
{

struct FGltfImportSettings
{
    float scale = 1.0f; // glTF units are meters
    bool convertAxes = true;
    bool flipV = true;
};

class GltfImporter
{
public:
    explicit GltfImporter(FGltfImportSettings settings = {});

    std::optional<FSceneImportResult> Import(const std::filesystem::path& path);

    const TArray<std::string>& GetErrors() const;
    const TArray<std::string>& GetWarnings() const;

private:
    bool CheckRequiredExtensions(const cgltf_data& data);

    void ReadTextures(const cgltf_data& data, const std::filesystem::path& baseDir, FSceneImportResult& result);
    void ReadMaterials(const cgltf_data& data, FSceneImportResult& result);
    void ReadMeshes(const cgltf_data& data, FSceneImportResult& result);
    void ReadNodes(const cgltf_data& data, FSceneImportResult& result);

    void ReadPrimitive(const cgltf_data& data, const cgltf_primitive& primitive, uint32 primitiveIndex, FMeshImportResult& mesh);
    bool ReadImageSource(const cgltf_data& data, const cgltf_image& image, const std::filesystem::path& baseDir, FTextureImportDesc& texture);

    void ClearMessages();

    void AddError(std::string message);
    void AddWarning(std::string message);

    FGltfImportSettings settings;

    TArray<std::string> errors;
    TArray<std::string> warnings;
};

}