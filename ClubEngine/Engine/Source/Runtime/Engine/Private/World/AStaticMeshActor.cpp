#include <Engine/World/AStaticMeshActor.h>

#include "Asset/IAssetResolver.h"
#include "Engine/World/UWorld.h"

namespace CE
{

AStaticMeshActor::AStaticMeshActor(const FTransform& transform, TObjectHandle<UStaticMesh> staticMeshHandle)
    : Super(transform)
{
    rootComponent = staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(transform, staticMeshHandle);
}

UStaticMeshComponent* AStaticMeshActor::GetStaticMeshComponent() const
{

}

}