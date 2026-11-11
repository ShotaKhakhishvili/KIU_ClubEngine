#pragma once

#include <Engine/Input/UInputTrigger.h>

namespace CE
{

class UInputTriggerReleased : public UInputTrigger
{
    GENERATED_BODY(UInputTriggerReleased, UInputTrigger)

public:
    ETriggerState UpdateState(const FInputActionValue& value, float dt) override;
};

}