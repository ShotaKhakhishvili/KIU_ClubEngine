#include <Engine/World/AStaticMeshActor.h>

#include "Asset/IAssetResolver.h"
#include "Engine/World/UWorld.h"

namespace CE
{

AStaticMeshActor::AStaticMeshActor(const FTransform& transform, TObjectHandle<UStaticMesh> staticMeshHandle)
    : Super(transform)
{
    staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(transform, staticMeshHandle);
    staticMeshComponent->SetParentComponent(rootComponent);
}

UStaticMeshComponent* AStaticMeshActor::GetStaticMeshComponent() const
{

}

}