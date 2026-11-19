#pragma once

#include <Engine/Actors/ALight.h>
#include <Engine/Components/UDirectionalLightComponent.h>

namespace CE
{

class ADirectionalLight : public ALight
{
    GENERATED_BODY(ADirectionalLight, ALight)

public:
    ADirectionalLight();

    UDirectionalLightComponent* GetDirectionalLightComponent() const;
};

}