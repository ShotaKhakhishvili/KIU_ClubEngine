#pragma once

#include <Asset/UStaticMesh.h>

#include <Engine/Actors/AActor.h>
#include <Engine/Components/UStaticMeshComponent.h>

namespace CE
{

class AStaticMeshActor : public AActor
{
    GENERATED_BODY(AStaticMeshActor, AActor)

public:
    AStaticMeshActor();

    UStaticMeshComponent* GetStaticMeshComponent() const;

private:
    UStaticMeshComponent* staticMeshComponent;
};

}
