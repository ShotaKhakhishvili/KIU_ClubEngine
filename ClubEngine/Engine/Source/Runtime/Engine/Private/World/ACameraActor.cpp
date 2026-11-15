#include "../../Public/Engine/World/ACameraActor.h"

namespace CE
{

IMPLEMENT_CLASS(ACameraActor)

ACameraActor::ACameraActor()
{
    cameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    rootComponent = cameraComponent;
}

UCameraComponent* ACameraActor::GetCameraComponent() const
{
    return cameraComponent;
}

}

