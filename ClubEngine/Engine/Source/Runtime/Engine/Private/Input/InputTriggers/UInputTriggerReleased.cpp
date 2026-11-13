#include <Engine/Input/InputTriggers/UInputTriggerReleased.h>

namespace CE
{

IMPLEMENT_CLASS(UInputTriggerReleased)

ETriggerState UInputTriggerReleased::UpdateState(const FInputActionValue& value, const float dt)
{
    (void)dt;

    if (IsActuated(value))
        return ETriggerState::Ongoing;

    if (IsActuated(lastInput))
        return ETriggerState::Triggered;

    return ETriggerState::None;
}

}