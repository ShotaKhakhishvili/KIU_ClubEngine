#pragma once

#include <Engine/World/AActor.h>
#include <Engine/World/UCameraComponent.h>

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
