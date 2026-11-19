#pragma once

#include <Engine/Actors/ALight.h>
#include <Engine/Components/UPointLightComponent.h>

namespace CE
{

class APointLight : public ALight
{
    GENERATED_BODY(APointLight, ALight)

public:
    APointLight();

    UPointLightComponent* GetPointLightComponent() const;
};

}