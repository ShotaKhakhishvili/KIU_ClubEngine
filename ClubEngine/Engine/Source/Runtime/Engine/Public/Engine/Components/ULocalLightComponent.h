#pragma once

#include <Engine/Components/ULightComponent.h>

namespace CE
{

class ULocalLightComponent : public ULightComponent
{
    GENERATED_BODY(ULocalLightComponent, ULightComponent)

public:
    ULocalLightComponent() = default;

    void SetAttenuationRadius(float newAttenuationRadius);
    float GetAttenuationRadius() const;
};

}
