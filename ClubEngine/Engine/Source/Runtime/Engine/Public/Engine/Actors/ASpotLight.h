#pragma once

#include <Engine/Actors/ALight.h>
#include <Engine/Components/USpotLightComponent.h>

namespace CE
{

class ASpotLight : public ALight
{
    GENERATED_BODY(ASpotLight, ALight)

public:
    ASpotLight();

    USpotLightComponent* GetSpotLightComponent() const;
};

}