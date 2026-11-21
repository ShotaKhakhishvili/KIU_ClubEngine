#pragma once

#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObject.h>

#include <Asset/FSubMesh.h>
#include <Asset/UMaterial.h>

#include <vector>
#include <string>

namespace CE
{

struct FMeshImportResult;

class UStaticMesh : public UObject
{
    GENERATED_BODY(UStaticMesh, UObject)

public:
    explicit UStaticMesh() = default;
    ~UStaticMesh() override;

    void Build(const FMeshImportResult& importResult);

    void SetMaterial(uint32 slot, TObjectHandle<UMaterial> material);
    const std::vector<TObjectHandle<UMaterial>>& GetMaterials() const;
    TObjectHandle<UMaterial> GetMaterial(uint32 slot) const;

    const std::vector<FSubMesh>& GetSubMeshes() const;
    RHI::VertexArrayHandle GetVertexArray() const;

    static void SetDefault(TObjectHandle<UStaticMesh> mesh);
    static TObjectHandle<UStaticMesh> GetDefault();

private:
    void Release();

    RHI::BufferHandle VBO;
    RHI::BufferHandle IBO;
    RHI::VertexArrayHandle VAO;

    std::vector<FSubMesh> subMeshes;
    std::vector<TObjectHandle<UMaterial>> materials;
};

}