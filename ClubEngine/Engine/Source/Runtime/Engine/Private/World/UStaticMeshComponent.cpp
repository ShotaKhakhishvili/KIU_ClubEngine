#include <Engine/World/UStaticMeshComponent.h>

#include "Engine/World/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{

IMPLEMENT_CLASS(UStaticMeshComponent)

void UStaticMeshComponent::SetStaticMesh(const TObjectHandle<UStaticMesh> newStaticMesh)
{
    staticMesh = newStaticMesh;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateStaticMesh(GetPrimitiveSceneProxy(), staticMesh);
}

TObjectHandle<UStaticMesh> UStaticMeshComponent::GetStaticMesh() const
{
    return staticMesh;
}

void UStaticMeshComponent::OnRegister()
{
    Super::OnRegister();

    GetOwner()->GetWorld()->GetScene().UpdateStaticMesh(GetPrimitiveSceneProxy(), staticMesh);
}

}