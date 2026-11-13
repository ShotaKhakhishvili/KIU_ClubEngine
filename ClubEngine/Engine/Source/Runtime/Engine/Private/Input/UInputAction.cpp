#include <Engine/Input/UInputAction.h>

namespace CE
{

IMPLEMENT_CLASS(UInputAction)

UInputAction::UInputAction(const EInputActionValueType valueType)
    : valueType(valueType)
{
}
EInputActionValueType UInputAction::GetValueType() const
{
    return valueType;
}

const TArray<std::unique_ptr<UInputTrigger>>& UInputAction::GetTriggers() const
{
    return triggers;
}

const TArray<std::unique_ptr<UInputModifier>>& UInputAction::GetModifiers() const
{
    return modifiers;
}

}
