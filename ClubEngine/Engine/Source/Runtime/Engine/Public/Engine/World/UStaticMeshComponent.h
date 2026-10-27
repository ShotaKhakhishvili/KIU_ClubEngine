#pragma once

#include <Engine/World/UPrimitiveComponent.h>

namespace CE
{

class UStaticMeshComponent : public UPrimitiveComponent
{
    GENERATED_BODY(UStaticMeshComponent, UPrimitiveComponent)

public:
    UStaticMeshComponent(AActor* owner, const FTransform& transform, TObjectHandle<UStaticMesh> staticMesh);

private:
    TObjectHandle<UStaticMesh> staticMesh;
};

}