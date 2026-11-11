#pragma once

#include <Engine/Input/UInputModifier.h>

namespace CE
{

// Multiplies each axis by its own factor (mouse sensitivity, inverting one axis, ...)
class UInputModifierScale : public UInputModifier
{
    GENERATED_BODY(UInputModifierScale, UInputModifier)

public:
    explicit UInputModifierScale(float uniformScale = 1.0f);
    explicit UInputModifierScale(const FVectorF& scale);

    FInputActionValue ModifyRaw(FInputActionValue value, float dt) override;

    FVectorF scale;
};

}