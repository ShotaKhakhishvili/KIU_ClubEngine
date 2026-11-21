#include <Core/ClubCore.h>

#include <CoreUObject/AssetRegistry.h>

#include <AssetImport/FSceneImportResult.h>
#include <AssetImport/GltfImporter.h>

#include <Asset/ImportAssetBuilder.h>

#include <cctype>
#include <optional>
#include <string>

namespace CE
{

namespace
{

constexpr const char* baseColorTextureName = "u_BaseColorTexture";
constexpr const char* metallicRoughnessTextureName = "u_MetallicRoughnessTexture";
constexpr const char* normalTextureName = "u_NormalTexture";
constexpr const char* occlusionTextureName = "u_OcclusionTexture";
constexpr const char* emissiveTextureName = "u_EmissiveTexture";

bool IsValidIndex(const int32 index, const size_t count)
{
    return index >= 0 && static_cast<size_t>(index) < count;
}

void AppendMesh(const FMeshImportResult& source, const FTransform& transform, FMeshImportResult& target)
{
    const uint32 baseVertex = static_cast<uint32>(target.vertices.size());
    const uint32 baseIndex = static_cast<uint32>(target.indices.size());

    const FVector scale = transform.Scale;

    const bool mirrored = scale.x * scale.y * scale.z < 0.0;

    const FVector inverseScale{
        scale.x != 0.0 ? 1.0 / scale.x : 0.0,
        scale.y != 0.0 ? 1.0 / scale.y : 0.0,
        scale.z != 0.0 ? 1.0 / scale.z : 0.0
    };

    target.vertices.reserve(target.vertices.size() + source.vertices.size());

    for (const FVertex& sourceVertex : source.vertices)
    {
        FVertex vertex = sourceVertex;

        vertex.coord = FVectorF(transform.TransformPoint(FVector(sourceVertex.coord)));
        vertex.normal = FVectorF(Normalize(transform.Rotation * (inverseScale * FVector(sourceVertex.normal))));

        const FVector tangent = FVector(sourceVertex.tangent.x, sourceVertex.tangent.y, sourceVertex.tangent.z);
        const float handedness = mirrored ? -sourceVertex.tangent.w : sourceVertex.tangent.w;

        vertex.tangent = {FVectorF(Normalize(transform.TransformDirection(tangent))), handedness};

        target.vertices.push_back(vertex);
    }

    target.indices.reserve(target.indices.size() + source.indices.size());

    for (size_t i = 0; i + 2 < source.indices.size(); i += 3)
    {
        const uint32 a = baseVertex + source.indices[i + 0];
        const uint32 b = baseVertex + source.indices[i + 1];
        const uint32 c = baseVertex + source.indices[i + 2];

        target.indices.push_back(a);
        target.indices.push_back(mirrored ? c : b);
        target.indices.push_back(mirrored ? b : c);
    }

    for (FMeshImportSection section : source.sections)
    {
        section.indexStart += baseIndex;
        target.sections.push_back(section);
    }
}

void AppendNodeMeshes(const FSceneImportResult& result, const int32 nodeIndex, const FTransform& parentTransform, FMeshImportResult& target)
{
    if (!IsValidIndex(nodeIndex, result.nodes.size()))
        return;

    const FNodeImportDesc& node = result.nodes[static_cast<size_t>(nodeIndex)];
    const FTransform worldTransform = Combine(parentTransform, node.localTransform);

    if (IsValidIndex(node.meshIndex, result.meshes.size()))
        AppendMesh(result.meshes[static_cast<size_t>(node.meshIndex)], worldTransform, target);

    for (const int32 child : node.children)
        AppendNodeMeshes(result, child, worldTransform, target);
}

}

TObjectHandle<UStaticMesh> ImportAssetBuilder::BuildStaticMesh(const FSceneImportResult& result)
{
    FMeshImportResult merged;

    if (result.rootNodes.size() == 1 && IsValidIndex(result.rootNodes[0], result.nodes.size()))
        merged.name = result.nodes[static_cast<size_t>(result.rootNodes[0])].name;
    else
        merged.name = "ImportedScene";

    for (const int32 rootIndex : result.rootNodes)
        AppendNodeMeshes(result, rootIndex, FTransform::Identity(), merged);

    if (result.nodes.empty())
    {
        for (const FMeshImportResult& mesh : result.meshes)
            AppendMesh(mesh, FTransform::Identity(), merged);
    }

    if (!merged.IsValid())
    {
        CE_LOG(Error, "ImportAssetBuilder: Scene has no mesh data to build a static mesh from");
        return {};
    }

    const TArray<TObjectHandle<UTexture>> textures = BuildTextures(result);
    const TArray<TObjectHandle<UMaterial>> materials = BuildMaterials(result, textures);

    return BuildMesh(merged, materials);
}

TArray<TObjectHandle<UTexture>> ImportAssetBuilder::BuildTextures(const FSceneImportResult& result)
{
    AssetRegistry& assets = GetAssetRegistry();

    TArray<TObjectHandle<UTexture>> textures(result.textures.size());

    for (size_t i = 0; i < result.textures.size(); ++i)
    {
        const FTextureImportDesc& textureDesc = result.textures[i];

        if (!textureDesc.HasSource())
            continue;

        const TObjectHandle<UTexture> handle = assets.Create<UTexture>();
        UTexture* texture = assets.Resolve(handle);

        if (texture == nullptr)
            continue;

        texture->SetName(textureDesc.name.c_str());

        bool built = false;

        if (textureDesc.IsEmbedded())
            built = texture->BuildFromData(textureDesc.encodedBytes, textureDesc.desc);
        else
            built = texture->BuildFromSourceFile(textureDesc.filePath, textureDesc.desc);

        if (!built)
        {
            assets.Destroy(handle.GetRaw());
            continue;
        }

        textures[i] = handle;
    }

    return textures;
}

TArray<TObjectHandle<UMaterial>> ImportAssetBuilder::BuildMaterials(const FSceneImportResult& result, const TArray<TObjectHandle<UTexture>>& textures)
{
    AssetRegistry& assets = GetAssetRegistry();

    TArray<TObjectHandle<UMaterial>> materials(result.materials.size());

    TObjectHandle<UShader> shader;

    if (const UMaterial* defaultMaterial = assets.Resolve(UMaterial::GetDefault()))
        shader = defaultMaterial->GetShader();
    else
        CE_LOG(Warning, "ImportAssetBuilder: No default material set, imported materials will have no shader");

    for (size_t i = 0; i < result.materials.size(); ++i)
    {
        const FMaterialImportDesc& materialDesc = result.materials[i];

        const TObjectHandle<UMaterial> handle = assets.Create<UMaterial>();
        UMaterial* material = assets.Resolve(handle);

        if (material == nullptr)
            continue;

        material->SetName(materialDesc.name.c_str());
        material->SetShader(shader);
        material->SetSettings(materialDesc.settings);

        auto SetTexture = [&](const char* uniformName, const FTextureRef& ref)
        {
            if (!IsValidIndex(ref.textureIndex, textures.size()))
                return;

            const TObjectHandle<UTexture> texture = textures[static_cast<size_t>(ref.textureIndex)];

            if (texture.IsValid())
                material->SetTexture(uniformName, texture);
        };

        SetTexture(baseColorTextureName, materialDesc.baseColorTexture);
        SetTexture(metallicRoughnessTextureName, materialDesc.metallicRoughnessTexture);
        SetTexture(normalTextureName, materialDesc.normalTexture);
        SetTexture(occlusionTextureName, materialDesc.occlusionTexture);
        SetTexture(emissiveTextureName, materialDesc.emissiveTexture);

        materials[i] = handle;
    }

    return materials;
}

TObjectHandle<UStaticMesh> ImportAssetBuilder::BuildMesh(const FMeshImportResult& meshResult, const TArray<TObjectHandle<UMaterial>>& materials)
{
    if (!meshResult.IsValid())
        return {};

    AssetRegistry& assets = GetAssetRegistry();

    const TObjectHandle<UStaticMesh> handle = assets.Create<UStaticMesh>();
    UStaticMesh* mesh = assets.Resolve(handle);

    if (mesh == nullptr)
        return {};

    mesh->SetName(meshResult.name.c_str());
    mesh->Build(meshResult);

    // Build creates one material slot per section
    for (size_t i = 0; i < meshResult.sections.size(); ++i)
        mesh->SetMaterial(static_cast<uint32>(i), PickMaterial(materials, meshResult.sections[i].materialIndex));

    return handle;
}

TObjectHandle<UMaterial> ImportAssetBuilder::PickMaterial(const TArray<TObjectHandle<UMaterial>>& materials, const int32 materialIndex)
{
    if (IsValidIndex(materialIndex, materials.size()) && materials[static_cast<size_t>(materialIndex)].IsValid())
        return materials[static_cast<size_t>(materialIndex)];

    return UMaterial::GetDefault();
}

TObjectHandle<UStaticMesh> ImportStaticMesh(const std::filesystem::path& path)
{
    std::string extension = path.extension().string();

    for (char& c : extension)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    if (extension != ".gltf" && extension != ".glb")
    {
        CE_LOG(Error, "Unsupported Mesh Format [{}]: {}", extension, path.string());
        return {};
    }

    GltfImporter importer;
    const std::optional<FSceneImportResult> result = importer.Import(path);

    for (const std::string& warning : importer.GetWarnings())
        CE_LOG(Warning, "glTF Import: {}", warning);

    for (const std::string& error : importer.GetErrors())
        CE_LOG(Error, "glTF Import: {}", error);

    if (!result.has_value())
        return {};

    ImportAssetBuilder builder;
    return builder.BuildStaticMesh(*result);
}

}