#pragma once

#include <Engine/Input/UInputTrigger.h>

namespace CE
{

class UInputTriggerPressed : public UInputTrigger
{
    GENERATED_BODY(UInputTriggerPressed, UInputTrigger)

public:
    ETriggerState UpdateState(const FInputActionValue& value, float dt) override;
};

}
