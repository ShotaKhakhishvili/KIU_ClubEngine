#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/TObjectHandle.h>

#include <Asset/UMaterial.h>
#include <Asset/UStaticMesh.h>
#include <Asset/UTexture.h>

#include <filesystem>

namespace CE
{

struct FMeshImportResult;
struct FSceneImportResult;

class ImportAssetBuilder
{
public:
    TObjectHandle<UStaticMesh> BuildStaticMesh(const FSceneImportResult& result);

private:
    TArray<TObjectHandle<UTexture>> BuildTextures(const FSceneImportResult& result);
    TArray<TObjectHandle<UMaterial>> BuildMaterials(const FSceneImportResult& result, const TArray<TObjectHandle<UTexture>>& textures);
    TObjectHandle<UStaticMesh> BuildMesh(const FMeshImportResult& meshResult, const TArray<TObjectHandle<UMaterial>>& materials);

    static TObjectHandle<UMaterial> PickMaterial(const TArray<TObjectHandle<UMaterial>>& materials, int32 materialIndex);
};

TObjectHandle<UStaticMesh> ImportStaticMesh(const std::filesystem::path& path);

}