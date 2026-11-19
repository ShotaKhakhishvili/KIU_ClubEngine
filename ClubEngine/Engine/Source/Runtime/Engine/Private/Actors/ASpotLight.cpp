#include <Engine/Actors/ASpotLight.h>

namespace CE
{

IMPLEMENT_CLASS(ASpotLight)

ASpotLight::ASpotLight()
{
    lightComponent = CreateDefaultSubobject<USpotLightComponent>("SpotLightComponent");
    rootComponent = lightComponent;
}

USpotLightComponent* ASpotLight::GetSpotLightComponent() const
{
    return static_cast<USpotLightComponent*>(lightComponent);
}

}