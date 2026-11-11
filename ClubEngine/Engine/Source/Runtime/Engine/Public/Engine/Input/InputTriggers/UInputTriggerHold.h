#pragma once

#include <Engine/Input/UInputTrigger.h>

namespace CE
{

class UInputTriggerHold : public UInputTrigger
{
    GENERATED_BODY(UInputTriggerHold, UInputTrigger)

public:
    explicit UInputTriggerHold(float holdTime = 0.5f, bool oneShot = false);

    ETriggerState UpdateState(const FInputActionValue& value, float dt) override;

    float   holdTime;
    bool    oneShot;

private:
    float   heldDuration    = 0.0f;
    bool    triggered       = false;
};

}