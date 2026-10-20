#pragma once

#include <Engine/World/UPrimitiveComponent.h>

namespace CE
{

class UStaticMeshComponent : public UPrimitiveComponent
{
    GENERATED_BODY(UStaticMeshComponent, UPrimitiveComponent)

public:
    UStaticMeshComponent(TObjectHandle<UStaticMesh> staticMesh);

private:
    TObjectHandle<UStaticMesh> staticMesh;
};

}