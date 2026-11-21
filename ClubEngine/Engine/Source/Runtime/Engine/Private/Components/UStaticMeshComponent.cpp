#include <CoreUObject/AssetRegistry.h>

#include <Engine/Components/UStaticMeshComponent.h>

#include <Engine/Actors/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

IMPLEMENT_CLASS(UStaticMeshComponent)

void UStaticMeshComponent::SetStaticMesh(const TObjectHandle<UStaticMesh> newStaticMesh)
{
    staticMesh = newStaticMesh;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdatePrimitiveStaticMesh(GetPrimitiveSceneProxy(), staticMesh);
}

TObjectHandle<UStaticMesh> UStaticMeshComponent::GetStaticMesh() const
{
    return staticMesh;
}

void UStaticMeshComponent::SetMaterial(const uint32 slot, const TObjectHandle<UMaterial> material)
{
    if (slot >= overrideMaterials.size())
        overrideMaterials.resize(slot + 1);

    overrideMaterials[slot] = material;

    PushOverrideMaterials();
}
TObjectHandle<UMaterial> UStaticMeshComponent::GetMaterial(const uint32 slot) const
{
    if (slot < overrideMaterials.size() && overrideMaterials[slot].IsValid())
        return overrideMaterials[slot];

    if (const UStaticMesh* mesh = Resolve(staticMesh))
        return mesh->GetMaterial(slot);

    return {};
}

uint32 UStaticMeshComponent::GetNumMaterials() const
{
    const UStaticMesh* mesh = Resolve(staticMesh);
    const size_t meshSlots = mesh ? mesh->GetMaterials().size() : 0;

    return static_cast<uint32>(std::max(meshSlots, overrideMaterials.size()));
}

void UStaticMeshComponent::ClearOverrideMaterials()
{
    overrideMaterials.clear();

    PushOverrideMaterials();
}

void UStaticMeshComponent::PushOverrideMaterials() const
{
    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdatePrimitiveOverrideMaterials(GetPrimitiveSceneProxy(), overrideMaterials);
}

void UStaticMeshComponent::OnRegister()
{
    Super::OnRegister();

    GetOwner()->GetWorld()->GetScene().UpdatePrimitiveStaticMesh(GetPrimitiveSceneProxy(), staticMesh);
}

}