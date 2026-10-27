#pragma once

#include <Asset/UStaticMesh.h>

#include <Engine/World/AActor.h>
#include <Engine/World/UStaticMeshComponent.h>

namespace CE
{

class AStaticMeshActor : public AActor
{
    GENERATED_BODY(AStaticMeshActor, AActor)

public:
    AStaticMeshActor(const FTransform& transform, TObjectHandle<UStaticMesh> staticMeshHandle);

    UStaticMeshComponent* GetStaticMeshComponent() const;

private:
    UStaticMeshComponent* staticMeshComponent;
};

}
