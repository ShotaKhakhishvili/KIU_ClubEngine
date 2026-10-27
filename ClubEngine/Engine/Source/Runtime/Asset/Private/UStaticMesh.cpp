#include <Core/ClubCore.h>

#include <RenderCore/RHI/IRHI.h>

#include <AssetImport/FMeshImportResult.h>

#include <Asset/UStaticMesh.h>

#include <optional>

namespace CE
{

UStaticMesh::UStaticMesh(std::string inName)
    : UObject(std::move(inName), "UStaticMesh")
{
}

const std::vector<FSubMesh>& UStaticMesh::GetSubMeshes() const { return subMeshes; }
const std::vector<TObjectHandle<UMaterial>>& UStaticMesh::GetMaterials() const { return materials; }

void UStaticMesh::AddSubMesh (const FSubMesh& subMesh)
{ 
    if(!subMesh.vertexArray.IsValid())
    {
        CE_LOG(Error, "Tried To Add SubMesh With Invalid Vertex Array To UStaticMesh: [{}]", this->GetName());
        return;
    }

    subMeshes.push_back(subMesh); 
}

void UStaticMesh::AddSubMeshWithImportResult(const FMeshImportResult* meshRes, const uint32 materialIndex)
{
    RHI::BufferDesc vbd{};
    vbd.type = RHI::BufferType::Vertex;
    vbd.usage = RHI::BufferUsage::StaticDraw;
    vbd.size = static_cast<uint32_t>(meshRes->vertices.size() * sizeof(FVertex));
    const auto vbHandle = RHI::Get().CreateBuffer(vbd, meshRes->vertices.data());

    RHI::BufferDesc ibd{};
    ibd.type = RHI::BufferType::Index;
    ibd.usage = RHI::BufferUsage::StaticDraw;
    ibd.size = static_cast<uint32_t>(meshRes->indices.size() * sizeof(uint32_t));
    const auto ibHandle = RHI::Get().CreateBuffer(ibd, meshRes->indices.data());

    RHI::VertexAttributeDesc a0{};
    a0.location = 0; a0.componentCount = 3; a0.dataType = RHI::ShaderDataType::Float;
    a0.stride = static_cast<uint32_t>(sizeof(FVertex));
    a0.offset = static_cast<uint32_t>(offsetof(FVertex, coord));

    RHI::VertexAttributeDesc a1 = a0; a1.location = 1; a1.offset = static_cast<uint32_t>(offsetof(FVertex, normal));
    RHI::VertexAttributeDesc a2 = a0; a2.location = 2; a2.offset = static_cast<uint32_t>(offsetof(FVertex, color));
    RHI::VertexAttributeDesc a3 = a0; a3.location = 3; a3.componentCount = 2; a3.offset = static_cast<uint32_t>(offsetof(FVertex, texUV));

    RHI::VertexArrayDesc vaDesc;
    vaDesc.attributes = { a0, a1, a2, a3 };
    const auto vaoHandle = RHI::Get().CreateVertexArray(vbHandle, ibHandle, vaDesc);

    FSubMesh subMesh{};
    subMesh.vertexArray = vaoHandle;
    subMesh.drawDesc.primitiveType = RHI::PrimitiveType::Triangles;
    subMesh.drawDesc.indexType = RHI::IndexType::UInt32;
    subMesh.drawDesc.indexCount = static_cast<uint32_t>(meshRes->indices.size());
    subMesh.drawDesc.indexOffset = 0;
    subMesh.materialIndex = materialIndex;

    AddSubMesh(subMesh);
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