#pragma once

#include <Engine/Components/UPointLightComponent.h>

namespace CE
{

class USpotLightComponent : public UPointLightComponent
{
    GENERATED_BODY(USpotLightComponent, UPointLightComponent)

public:
    USpotLightComponent();

    void SetInnerConeAngle(float newInnerConeAngle);
    float GetInnerConeAngle() const;

    void SetOuterConeAngle(float newOuterConeAngle);
    float GetOuterConeAngle() const;
};

}
