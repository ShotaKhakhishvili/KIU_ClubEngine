#include <Engine/Actors/APointLight.h>

namespace CE
{

IMPLEMENT_CLASS(APointLight)

APointLight::APointLight()
{
    lightComponent = CreateDefaultSubobject<UPointLightComponent>("PointLightComponent");
    rootComponent = lightComponent;
}

UPointLightComponent* APointLight::GetPointLightComponent() const
{
    return static_cast<UPointLightComponent*>(lightComponent);
}

}