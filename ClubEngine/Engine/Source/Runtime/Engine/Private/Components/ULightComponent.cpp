#include <Engine/Components/ULightComponent.h>

#include <Engine/Actors/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{
IMPLEMENT_CLASS(ULightComponent)

void ULightComponent::OnTransformUpdated()
{
    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateLightTransform(proxyHandle, GetWorldTransform());
}

void ULightComponent::OnVisibilityChanged()
{
    Super::OnVisibilityChanged();

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateLightVisibility(proxyHandle, IsVisible());
}

void ULightComponent::OnRegister()
{
    Super::OnRegister();

    FLightSceneProxy proxy;
    proxy.isVisible = IsVisible();
    proxy.localToWorld = static_cast<FMatrix4x4F>(GetWorldTransform().ToMatrix());
    proxy.lightParameters = lightParameters;
    proxy.lightType = lightType;
    
    proxyHandle = GetOwner()->GetWorld()->GetScene().AddLight(proxy);
}

void ULightComponent::OnUnregister()
{
    GetOwner()->GetWorld()->GetScene().RemoveLight(proxyHandle);
    proxyHandle = {};

    Super::OnUnregister();
}

const FLightParameters& ULightComponent::GetLightParameters()   const { return lightParameters;                     }
const FVectorF& ULightComponent::GetLightColor()                const { return lightParameters.color;               }
float ULightComponent::GetIntensity()                           const { return lightParameters.intensity;           }
ELightType ULightComponent::GetLightType()                      const { return lightType;                           }

void ULightComponent::SetLightParameters(const FLightParameters& newLightParameters)
{
    lightParameters = newLightParameters;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateLightParameters(proxyHandle, lightParameters);
}

void ULightComponent::SetLightColor(const FVectorF& newColor)
{
    lightParameters.color = newColor;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateLightColor(proxyHandle, newColor);
}

void ULightComponent::SetIntensity(const float newIntensity)
{
    lightParameters.intensity = newIntensity;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateLightIntensity(proxyHandle, newIntensity);
}

}