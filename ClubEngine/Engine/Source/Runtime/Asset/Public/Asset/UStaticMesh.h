#pragma once

#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObject.h>

#include <Asset/SubMesh.h>
#include <Asset/UMaterial.h>

#include <vector>
#include <string>

namespace CE
{

class UStaticMesh : public UObject
{
public:
    explicit UStaticMesh(std::string inName = "UStaticMesh");

    const std::vector<SubMesh>& GetSubMeshes() const;
    const std::vector<TObjectHandle<UMaterial>>& GetMaterials() const;

    void AddSubMesh(const SubMesh& subMesh);
    void AddMaterial(TObjectHandle<UMaterial> material);

private:
    std::vector<SubMesh> subMeshes;
    std::vector<TObjectHandle<UMaterial>> materials;
};

}