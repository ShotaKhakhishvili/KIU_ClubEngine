#include <Engine/Input/UInputTrigger.h>

namespace CE
{

IMPLEMENT_CLASS(UInputTrigger)

ETriggerState UInputTrigger::Update(const FInputActionValue& value, const float dt)
{
    const ETriggerState newState = UpdateState(value, dt);

    lastInput = value;

    return newState;
}

void UInputTrigger::SetActuationThreshold(const float newThreshold)
{
    actuationThreshold = newThreshold;
}

float UInputTrigger::GetActuationThreshold() const
{
    return actuationThreshold;
}

bool UInputTrigger::IsActuated(const FInputActionValue& value) const
{
    return Length(value.value) > actuationThreshold;
}

}
