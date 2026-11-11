#include <Engine/Input/InputTriggers/UInputTriggerHold.h>

namespace CE
{

UInputTriggerHold::UInputTriggerHold(const float holdTime, const bool oneShot)
    : holdTime(holdTime), oneShot(oneShot)
{
}

ETriggerState UInputTriggerHold::UpdateState(const FInputActionValue& value, const float dt)
{
    if (!IsActuated(value))
    {
        heldDuration = 0.0f;
        triggered = false;
        return ETriggerState::None;
    }

    heldDuration += dt;
    if (heldDuration < holdTime)
        return ETriggerState::Ongoing;

    const bool firstTrigger = !triggered;
    triggered = true;

    return firstTrigger || oneShot ? ETriggerState::None : ETriggerState::Triggered;
}

}