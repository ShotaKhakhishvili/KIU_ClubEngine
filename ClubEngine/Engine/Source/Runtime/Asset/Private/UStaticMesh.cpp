#include <Core/ClubCore.h>

#include <RenderCore/RHI/IRHI.h>

#include <AssetImport/FMeshImportResult.h>

#include <Asset/UStaticMesh.h>

#include <optional>

namespace CE
{

namespace
{
TObjectHandle<UStaticMesh> GDefaultStaticMesh;
}

IMPLEMENT_CLASS(UStaticMesh);

const std::vector<FSubMesh>& UStaticMesh::GetSubMeshes()                    const { return subMeshes;           }
RHI::VertexArrayHandle UStaticMesh::GetVertexArray()                        const { return VAO;                 }
const std::vector<TObjectHandle<UMaterial>>& UStaticMesh::GetMaterials()    const { return materials;           }
TObjectHandle<UStaticMesh> UStaticMesh::GetDefault()                              { return GDefaultStaticMesh;  }

void UStaticMesh::SetDefault(const TObjectHandle<UStaticMesh> mesh)
{
    GDefaultStaticMesh = mesh;
}

UStaticMesh::~UStaticMesh()
{
    Release();
}

void UStaticMesh::Build(const FMeshImportResult& importResult)
{
    if (importResult.sections.empty() || importResult.vertices.empty() || importResult.indices.empty())
    {
        CE_LOG(Error, "UStaticMesh::Build: Invalid data when building a static mesh");
        return;
    }

    Release();

    // VBO
    RHI::BufferDesc vbd{};
    vbd.type = RHI::BufferType::Vertex;
    vbd.usage = RHI::BufferUsage::StaticDraw;
    vbd.size = static_cast<uint32_t>(importResult.vertices.size() * sizeof(FVertex));
    VBO = RHI::Get().CreateBuffer(vbd, importResult.vertices.data());

    // IBO
    RHI::BufferDesc ibd{};
    ibd.type = RHI::BufferType::Index;
    ibd.usage = RHI::BufferUsage::StaticDraw;
    ibd.size = static_cast<uint32_t>(importResult.indices.size() * sizeof(uint32_t));
    IBO = RHI::Get().CreateBuffer(ibd, importResult.indices.data());

    // VAO
    RHI::VertexAttributeDesc a0{};
    a0.location = 0; a0.componentCount = 3; a0.dataType = RHI::ShaderDataType::Float;
    a0.stride = static_cast<uint32_t>(sizeof(FVertex));
    a0.offset = static_cast<uint32_t>(offsetof(FVertex, coord));

    RHI::VertexAttributeDesc a1 = a0; a1.location = 1; a1.offset = static_cast<uint32_t>(offsetof(FVertex, normal));
    RHI::VertexAttributeDesc a2 = a0; a2.location = 2; a2.offset = static_cast<uint32_t>(offsetof(FVertex, color));
    RHI::VertexAttributeDesc a3 = a0; a3.location = 3; a3.componentCount = 2; a3.offset = static_cast<uint32_t>(offsetof(FVertex, texUV));
    RHI::VertexAttributeDesc a4 = a0; a4.location = 4; a4.componentCount = 4; a4.offset = static_cast<uint32_t>(offsetof(FVertex, tangent));

    RHI::VertexArrayDesc vaDesc;
    vaDesc.attributes = { a0, a1, a2, a3, a4 };
    VAO = RHI::Get().CreateVertexArray(VBO, IBO, vaDesc);

    // Submeshes
    for (uint32 i = 0; i < importResult.sections.size(); i++)
    {
        const FMeshImportSection& section = importResult.sections[i];
        FSubMesh subMesh{};

        subMesh.drawDesc.primitiveType = RHI::PrimitiveType::Triangles;
        subMesh.drawDesc.indexType = RHI::IndexType::UInt32;
        subMesh.drawDesc.indexOffset = section.indexStart;
        subMesh.drawDesc.indexCount = section.indexCount;
        subMesh.materialIndex = i;

        subMeshes.push_back(subMesh);
    }

    materials.assign(subMeshes.size(), {});
}

void UStaticMesh::Release()
{
    if (VAO.IsValid())
        RHI::Get().DestroyVertexArray(VAO);

    if (IBO.IsValid())
        RHI::Get().DestroyBuffer(IBO);

    if (VBO.IsValid())
        RHI::Get().DestroyBuffer(VBO);

    VAO = {};
    IBO = {};
    VBO = {};

    subMeshes.clear();
    materials.clear();
}

void UStaticMesh::SetMaterial(const uint32 slot, const TObjectHandle<UMaterial> material)
{
    if (slot < materials.size())
        materials[slot] = material;
}

TObjectHandle<UMaterial> UStaticMesh::GetMaterial(const uint32 slot) const
{
    if (slot < materials.size())
        return materials[slot];
    return {};
}

}