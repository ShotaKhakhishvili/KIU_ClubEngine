#pragma once

#include <Engine/Input/UInputModifier.h>

namespace CE
{

class UInputModifierNegate : public UInputModifier
{
    GENERATED_BODY(UInputModifierNegate, UInputModifier)

public:
    FInputActionValue ModifyRaw(FInputActionValue value, float dt) override;
};

}
