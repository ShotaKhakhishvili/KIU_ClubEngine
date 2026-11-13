#include <Engine/World/AStaticMeshActor.h>

#include "Asset/IAssetResolver.h"
#include "Engine/World/UWorld.h"

namespace CE
{

IMPLEMENT_CLASS(AStaticMeshActor)

AStaticMeshActor::AStaticMeshActor(const FTransform& transform, TObjectHandle<UStaticMesh> staticMeshHandle)
    : Super(transform)
{
    rootComponent = CreateDefaultSubobject<UStaticMeshComponent>(transform, staticMeshHandle);
}

UStaticMeshComponent* AStaticMeshActor::GetStaticMeshComponent() const
{
    return dynamic_cast<UStaticMeshComponent*>(rootComponent);
}

}