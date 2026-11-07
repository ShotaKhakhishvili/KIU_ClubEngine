#include <Engine/Input/FActionKeyMapping.h>

#include <algorithm>

namespace CE
{

FActionKeyMapping::FActionKeyMapping(const EKey key, UInputAction* inputAction,
    const TArray<UInputTrigger*>& triggers, const TArray<UInputModifier*>& modifiers)
        : key(key), inputAction(inputAction)
{
    for (const auto& modifier : modifiers)
        AddModifier(modifier);
    for (const auto& trigger : triggers)
        AddTrigger(trigger);
}

void FActionKeyMapping::SetKey(const EKey key)
{
    this->key = key;
}

EKey FActionKeyMapping::GetKey() const
{
    return key;
}

void FActionKeyMapping::SetInputAction(UInputAction* const inputAction)
{
    this->inputAction = inputAction;
}

UInputAction* FActionKeyMapping::GetInputAction() const
{
    return inputAction;
}

bool FActionKeyMapping::AddTrigger(UInputTrigger* trigger)
{
    if (!trigger)
        return false;

    if (std::ranges::find(triggers, trigger) == triggers.end())
    {
        triggers.push_back(trigger);
        return true;
    }
    return false;
}

bool FActionKeyMapping::AddModifier(UInputModifier* modifier)
{
    if (!modifier)
        return false;

    if (std::ranges::find(modifiers, modifier) == modifiers.end())
    {
        modifiers.push_back(modifier);
        return true;
    }
    return false;
}

const TArray<UInputModifier*>& FActionKeyMapping::GetModifiers() const
{
    return modifiers;
}

const TArray<UInputTrigger*>& FActionKeyMapping::GetTriggers() const
{
    return triggers;
}

}