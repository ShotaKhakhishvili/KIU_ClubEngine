#pragma once

#include <Engine/Components/ULocalLightComponent.h>

namespace CE
{

class UPointLightComponent : public ULocalLightComponent
{
    GENERATED_BODY(UPointLightComponent, ULocalLightComponent)

public:
    UPointLightComponent();
};

}
