#include <Engine/Components/USpotLightComponent.h>
#include <Engine/Actors/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

IMPLEMENT_CLASS(USpotLightComponent)

float USpotLightComponent::GetInnerConeAngle()                      const { return lightParameters.innerConeAngle;      }
float USpotLightComponent::GetOuterConeAngle()                      const { return lightParameters.outerConeAngle;      }

USpotLightComponent::USpotLightComponent()
{
    lightType = ELightType::Spot;
}

void USpotLightComponent::SetInnerConeAngle(const float newInnerConeAngle)
{
    lightParameters.innerConeAngle = newInnerConeAngle;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateLightInnerConeAngle(proxyHandle, newInnerConeAngle);
}

void USpotLightComponent::SetOuterConeAngle(const float newOuterConeAngle)
{
    lightParameters.outerConeAngle = newOuterConeAngle;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateLightOuterConeAngle(proxyHandle, newOuterConeAngle);
}

}