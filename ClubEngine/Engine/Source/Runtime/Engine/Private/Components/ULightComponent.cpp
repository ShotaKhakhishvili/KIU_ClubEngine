#include <Engine/Components/ULightComponent.h>

#include "Engine/Actors/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{
IMPLEMENT_CLASS(ULightComponent)

void ULightComponent::OnRegister()
{
    Super::OnRegister();

    FLightSceneProxy proxy;
    proxy.isVisible = IsVisible();
    proxy.localToWorld = static_cast<FMatrix4x4F>(GetWorldTransform().ToMatrix());

    proxyHandle = GetOwner()->GetWorld()->GetScene().AddLight(proxy);
}

void ULightComponent::OnUnregister()
{
    Super::OnUnregister();

    GetOwner()->GetWorld()->GetScene().RemoveLight(proxyHandle);
}

}