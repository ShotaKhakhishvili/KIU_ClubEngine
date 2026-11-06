#include <Engine/Input/UInputAction.h>

namespace CE
{

UInputAction::UInputAction(const EInputActionValueType valueType, TArray<UInputTrigger*> triggers, TArray<UInputModifier*> modifiers)
    : valueType(valueType), modifiers(std::move(modifiers)), triggers(std::move(triggers))
{
}
EInputActionValueType UInputAction::GetValueType() const
{
    return valueType;
}

const TArray<UInputTrigger*>& UInputAction::GetTriggers() const
{
    return triggers;
}

const TArray<UInputModifier*>& UInputAction::GetModifiers() const
{
    return modifiers;
}

}
