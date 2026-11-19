#include <Engine/Components/ULocalLightComponent.h>
#include <Engine/Actors/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

IMPLEMENT_CLASS(ULocalLightComponent)

float ULocalLightComponent::GetAttenuationRadius()                   const { return lightParameters.attenuationRadius;   }

void ULocalLightComponent::SetAttenuationRadius(const float newAttenuationRadius)
{
    lightParameters.attenuationRadius = newAttenuationRadius;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateLightAttenuationRadius(proxyHandle, newAttenuationRadius);
}

}