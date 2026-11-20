#pragma once

#include <Core/ClubCore.h>
#include <Core/Types/RHITypes.h>

#include <AssetImport/FMeshImportResult.h>

#include <filesystem>
#include <string>

namespace CE
{

struct FTextureImportDesc
{
    std::string name;

    // One of these is set: filePath for external images, encodedBytes for embedded ones
    std::filesystem::path filePath;
    TArray<uint8> encodedBytes;
    std::string mimeType;

    RHI::TextureDesc desc;

    bool IsEmbedded() const
    {
        return !encodedBytes.empty();
    }

    bool HasSource() const
    {
        return IsEmbedded() || !filePath.empty();
    }
};

struct FTextureRef
{
    int32 textureIndex = -1;
    uint32 texCoord = 0;

    bool IsValid() const
    {
        return textureIndex >= 0;
    }
};

struct FMaterialImportDesc
{
    std::string name;

    RHI::FMaterialSettings settings;

    FTextureRef baseColorTexture;
    FTextureRef metallicRoughnessTexture; // G = roughness, B = metallic
    FTextureRef normalTexture;
    FTextureRef occlusionTexture; // R = occlusion
    FTextureRef emissiveTexture;
};

struct FNodeImportDesc
{
    std::string name;

    FTransform localTransform;

    int32 parent = -1;
    TArray<int32> children;

    int32 meshIndex = -1;
};

struct FSceneImportResult
{
    TArray<FMeshImportResult> meshes;
    TArray<FMaterialImportDesc> materials;
    TArray<FTextureImportDesc> textures;
    TArray<FNodeImportDesc> nodes;

    TArray<int32> rootNodes;

    bool IsValid() const
    {
        for (const FMeshImportResult& mesh : meshes)
        {
            if (mesh.IsValid())
                return true;
        }

        return false;
    }
};

}