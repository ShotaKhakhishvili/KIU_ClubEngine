#include <Engine/Actors/ALight.h>

namespace CE
{

IMPLEMENT_CLASS(ALight)

ULightComponent* ALight::GetLightComponent() const
{
    return lightComponent;
}

}