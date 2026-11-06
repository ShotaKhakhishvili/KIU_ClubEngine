#include <Engine/Input/InputModifiers/UInputModifierNegate.h>

namespace CE
{

FInputActionValue UInputModifierNegate::ModifyRaw(const FInputActionValue value, const float dt)
{
    (void)dt;
    return FInputActionValue{FVectorF(-value.value), value.type};
}

}