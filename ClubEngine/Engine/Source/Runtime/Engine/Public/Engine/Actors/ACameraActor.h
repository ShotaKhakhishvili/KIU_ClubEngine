#pragma once

#include <Engine/Actors/AActor.h>
#include <Engine/Components/UCameraComponent.h>

namespace CE
{

class ACameraActor : public AActor
{
    GENERATED_BODY(ACameraActor, AActor)

public:
    ACameraActor();

    UCameraComponent* GetCameraComponent() const;

private:
    UCameraComponent* cameraComponent;
};
}
