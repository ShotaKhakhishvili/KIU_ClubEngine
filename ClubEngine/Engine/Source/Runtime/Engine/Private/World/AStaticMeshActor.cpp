#include <Engine/World/AStaticMeshActor.h>

#include "Asset/IAssetResolver.h"
#include "Engine/World/UWorld.h"

namespace CE
{

IMPLEMENT_CLASS(AStaticMeshActor)

AStaticMeshActor::AStaticMeshActor()
{
    staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    rootComponent = staticMeshComponent;
}

UStaticMeshComponent* AStaticMeshActor::GetStaticMeshComponent() const
{
    return staticMeshComponent;
}

}