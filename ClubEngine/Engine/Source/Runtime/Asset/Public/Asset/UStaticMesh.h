#pragma once

#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObject.h>

#include <Asset/FSubMesh.h>
#include <Asset/UMaterial.h>

#include <vector>
#include <string>

namespace CE
{

class UStaticMesh : public UObject
{
    GENERATED_BODY(UStaticMesh, UObject)

public:
    explicit UStaticMesh(std::string inName = "UStaticMesh");

    const std::vector<FSubMesh>& GetSubMeshes() const;
    const std::vector<TObjectHandle<UMaterial>>& GetMaterials() const;

    void AddSubMesh(const FSubMesh& subMesh);
    void AddMaterial(TObjectHandle<UMaterial> material);

private:
    std::vector<FSubMesh> subMeshes;
    std::vector<TObjectHandle<UMaterial>> materials;
};

}