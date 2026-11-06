#pragma once

#include <Engine/Input/UInputModifier.h>
#include <Engine/Input/UInputTrigger.h>

#include <CoreUObject/UObject.h>

namespace CE
{

class UInputAction : public UObject
{
    GENERATED_BODY(UInputAction, UObject)

public:
    UInputAction(EInputActionValueType valueType, TArray<UInputTrigger*> triggers, TArray<UInputModifier*> modifiers);

    EInputActionValueType GetValueType() const;
    const TArray<UInputTrigger*>& GetTriggers() const;
    const TArray<UInputModifier*>& GetModifiers() const;

private:
    EInputActionValueType valueType;
    TArray<UInputModifier*> modifiers;
    TArray<UInputTrigger*> triggers;
};

}
