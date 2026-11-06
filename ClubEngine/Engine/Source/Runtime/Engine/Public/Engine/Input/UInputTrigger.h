#pragma once

#include <CoreUObject/UObject.h>

#include <Engine/Input/Types/FInputActionValue.h>
#include <Engine/Input/Types/ETriggerState.h>

namespace CE
{
class UInputTrigger : public UObject
{
    GENERATED_BODY(UInputTrigger, UObject)

public:
    ETriggerState Update(const FInputActionValue& value, float dt);

    void SetActuationThreshold(float newThreshold);
    float GetActuationThreshold() const;

protected:
    virtual ETriggerState UpdateState(const FInputActionValue& value, float dt) = 0;

    bool IsActuated(const FInputActionValue& value) const;

    FInputActionValue lastInput = {};
    float actuationThreshold = 0.5f;
};

}
