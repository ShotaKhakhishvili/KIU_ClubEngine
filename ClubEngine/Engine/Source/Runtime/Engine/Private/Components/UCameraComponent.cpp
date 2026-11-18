#include <Engine/Components/UCameraComponent.h>

#include <Engine/Actors/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

IMPLEMENT_CLASS(UCameraComponent)
FMinimalViewInfo UCameraComponent::GetCameraView() const
{
    return FMinimalViewInfo{
        .pos = GetWorldLocation(),
        .rot = GetWorldRotationInQuat(),
        .fov = fov,
        .zNear = zNear,
        .zFar = zFar
    };
}

void UCameraComponent::OnRegister()
{
    USceneComponent::OnRegister();

    GetOwner()->GetWorld()->AddViewTarget(this);
}

void UCameraComponent::OnUnregister()
{
    Super::OnUnregister();

    GetOwner()->GetWorld()->RemoveViewTarget(this);
}

}