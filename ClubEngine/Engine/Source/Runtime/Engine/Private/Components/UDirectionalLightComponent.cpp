#include <Engine/Components/UDirectionalLightComponent.h>

namespace CE
{

IMPLEMENT_CLASS(UDirectionalLightComponent)

UDirectionalLightComponent::UDirectionalLightComponent()
{
    lightType = ELightType::Directional;
}

}