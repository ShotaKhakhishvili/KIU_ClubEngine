#include <Engine/AssetContext.h>

#include "AssetImport/ObjMeshImporter.h"
#include "Project/ActiveProject.h"
#include "Renderer/Scene/FPrimitiveSceneProxy.h"

#include <utility>

namespace CE
{
class WorldSystem;

void AssetContext::Destroy(UObjectHandle handle) const
{
    registry.Destroy(handle);
}

UShader* AssetContext::Resolve(TObjectHandle<UShader> handle) const
{
    return registry.Resolve(handle);
}

UTexture* AssetContext::Resolve(TObjectHandle<UTexture> handle) const
{
    return registry.Resolve(handle);
}

UMaterial* AssetContext::Resolve(TObjectHandle<UMaterial> handle) const
{
    return registry.Resolve(handle);
}

UStaticMesh* AssetContext::Resolve(TObjectHandle<UStaticMesh> handle) const
{
    return registry.Resolve(handle);
}

TObjectHandle<UMaterial> AssetContext::GetDefaultMaterial() const
{
    return defaultMaterial;
}

TObjectHandle<UShader> AssetContext::GetDefaultShader() const
{
    return defaultShader;
}
TObjectHandle<UStaticMesh> AssetContext::GetDebugStaticMesh() const
{
    return debugStaticMesh;
}
void AssetContext::CreateEngineDefaults()
{
    using std::filesystem::path;

    const path vertPath = GetActiveProject().GetEnginePath() / path("Engine/Source/Runtime/Engine/Public/Engine/Shaders/Common/Default/default.vert");
    const path fragPath = GetActiveProject().GetEnginePath() / path("Engine/Source/Runtime/Engine/Public/Engine/Shaders/Common/Default/default.frag");

    RHI::ShaderDesc shaderDesc;
    shaderDesc.stages = {
                { RHI::ShaderStage::Vertex,   FileIO::ReadFileWithIncludes(vertPath) },
                { RHI::ShaderStage::Fragment, FileIO::ReadFileWithIncludes(fragPath) }
    };

    defaultShader = Create<UShader>(shaderDesc, "DefaultShader");
    UShader* shader = Resolve(defaultShader);
    if (shader && !shader->IsReady())
        shader->Load();

    defaultMaterial = Create<UMaterial>(defaultShader, "DefaultMaterial");
    UMaterial* material = Resolve(defaultMaterial);
    if (material)
    {
        material->SetVec3("u_LightPos", 5.0f, 10.0f, 20.0f);
        material->SetBool("u_UseTexture", false);
    }

    const path objPath = GetActiveProject().GetEnginePath() / "Engine/Source/Runtime/Engine/Public/Engine/Geometry/debugStaticMesh.obj";
    ObjMeshImporter importer;
    auto importRes = importer.Import(objPath);
    if (!importRes.has_value())
    {
        CE_LOG(Warning, "Debug mesh import failed: {}", objPath.string());
        return;
    }

    const FMeshImportResult& meshRes = importRes.value();

    debugStaticMesh = Create<UStaticMesh>(std::string("DebugMesh"));
    UStaticMesh* staticMesh = Resolve(debugStaticMesh);

    if (staticMesh)
    {
        staticMesh->AddMaterial(GetDefaultMaterial());
        staticMesh->AddSubMeshWithImportResult(&meshRes);
    }
}

}