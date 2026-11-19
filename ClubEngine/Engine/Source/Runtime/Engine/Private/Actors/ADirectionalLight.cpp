#include <Engine/Actors/ADirectionalLight.h>

namespace CE
{

IMPLEMENT_CLASS(ADirectionalLight)

ADirectionalLight::ADirectionalLight()
{
    lightComponent = CreateDefaultSubobject<UDirectionalLightComponent>("DirectionalLightComponent");
    rootComponent = lightComponent;
}

UDirectionalLightComponent* ADirectionalLight::GetDirectionalLightComponent() const
{
    return static_cast<UDirectionalLightComponent*>(lightComponent);
}

}