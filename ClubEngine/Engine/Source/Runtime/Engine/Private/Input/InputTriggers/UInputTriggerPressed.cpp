#include <Engine/Input/InputTriggers/UInputTriggerPressed.h>

namespace CE
{

ETriggerState UInputTriggerPressed::UpdateState(const FInputActionValue& value, const float dt)
{
    (void)dt;

    if (!IsActuated(lastInput) && IsActuated(value))
        return ETriggerState::Triggered;

    return ETriggerState::None;
}

}