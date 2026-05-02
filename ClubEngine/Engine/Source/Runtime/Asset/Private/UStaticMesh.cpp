#include <Core/ClubCore.h>

#include <Asset/UStaticMesh.h>

namespace CE
{

UStaticMesh::UStaticMesh(std::string inName)
    : UObject(std::move(inName))
{
}

const std::vector<SubMesh>& UStaticMesh::GetSubMeshes() const { return subMeshes; }
const std::vector<TObjectHandle<UMaterial>>& UStaticMesh::GetMaterials() const { return materials; }

void UStaticMesh::AddSubMesh (const SubMesh& subMesh) 
{ 
    if(!subMesh.vertexArray.IsValid())
    {
        CE_LOG(Error, "Tried To Add SubMesh With Invalid Vertex Array To UStaticMesh: [{}]", this->GetName());
        return;
    }

    subMeshes.push_back(subMesh); 
}

void UStaticMesh::AddMaterial (TObjectHandle<UMaterial> material) 
{
    if(!material.IsValid())
    {
        CE_LOG(Error, "Tried To Add Invalid Material To UStaticMesh: [{}]", this->GetName());
        return;
    }
    materials.push_back(material); 
}

}