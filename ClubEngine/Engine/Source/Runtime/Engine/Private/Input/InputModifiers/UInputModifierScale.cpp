#include <Engine/Input/InputModifiers/UInputModifierScale.h>

namespace CE
{

UInputModifierScale::UInputModifierScale(const float uniformScale)
    : scale(uniformScale, uniformScale, uniformScale)
{
}

UInputModifierScale::UInputModifierScale(const FVectorF& scale)
    : scale(scale)
{
}

FInputActionValue UInputModifierScale::ModifyRaw(const FInputActionValue value, const float dt)
{
    (void)dt;

    const FVectorF& v = value.value;
    return FInputActionValue{FVectorF(v.x * scale.x, v.y * scale.y, v.z * scale.z), value.type};
}

}