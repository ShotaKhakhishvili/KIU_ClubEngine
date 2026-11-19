#pragma once

#include <Engine/Actors/AActor.h>
#include <Engine/Components/ULightComponent.h>

namespace CE
{

class ALight : public AActor
{
    GENERATED_BODY(ALight, AActor)

public:
    ULightComponent* GetLightComponent() const;

protected:
    ALight() = default;

    ULightComponent* lightComponent = nullptr;
};

}