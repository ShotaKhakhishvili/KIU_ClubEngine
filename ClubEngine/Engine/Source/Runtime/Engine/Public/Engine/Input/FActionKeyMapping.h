#pragma once

#include <Core/Types/InputTypes.h>

#include <Engine/Input/UInputTrigger.h>
#include <Engine/Input/UInputModifier.h>
#include <Engine/Input/UInputAction.h>

namespace CE
{
class FActionKeyMapping
{
public:
    FActionKeyMapping(EKey key, UInputAction* inputAction, const TArray<UInputTrigger*>& triggers, const TArray<UInputModifier*>& modifiers);

    void SetKey(EKey key);
    EKey GetKey() const;

    void SetInputAction(UInputAction* inputAction);
    UInputAction* GetInputAction() const;

    bool AddTrigger(UInputTrigger* trigger);
    bool AddModifier(UInputModifier* modifier);

    const TArray<UInputModifier*>& GetModifiers() const;
    const TArray<UInputTrigger*>& GetTriggers() const;

private:
    EKey key;

    UInputAction* inputAction{};
    TArray<UInputTrigger*> triggers;
    TArray<UInputModifier*> modifiers;
};
}
