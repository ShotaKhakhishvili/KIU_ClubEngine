#pragma once

#include <Engine/World/UPrimitiveComponent.h>

namespace CE
{

class UStaticMeshComponent : public UPrimitiveComponent
{
    GENERATED_BODY(UStaticMeshComponent, UPrimitiveComponent)

public:
    UStaticMeshComponent(UStaticMesh* staticMesh);

private:
    UStaticMesh* staticMesh;
};

}