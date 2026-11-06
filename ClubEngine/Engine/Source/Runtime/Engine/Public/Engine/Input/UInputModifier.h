#pragma once

#include <CoreUObject/UObject.h>

#include <Engine/Input/Types/FInputActionValue.h>

namespace CE
{

class UInputModifier : public UObject
{
    GENERATED_BODY(UInputModifier, UObject)

public:
    virtual FInputActionValue ModifyRaw(FInputActionValue value, float dt) = 0;
};

}
