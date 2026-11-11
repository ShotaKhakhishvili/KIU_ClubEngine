#include <Engine/World/UStaticMeshComponent.h>

#include "Engine/World/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{

UStaticMeshComponent::UStaticMeshComponent(AActor* owner, const FTransform& transform, const TObjectHandle<UStaticMesh> staticMesh)
    : Super(owner, transform), staticMesh(staticMesh)
{
    GetOwner()->GetWorld()->GetScene().UpdateStaticMesh(GetPrimitiveSceneProxy(), staticMesh);
}


}