#include <Engine/Components/UPointLightComponent.h>

namespace CE
{

IMPLEMENT_CLASS(UPointLightComponent)

UPointLightComponent::UPointLightComponent()
{
    lightType = ELightType::Point;
}

}