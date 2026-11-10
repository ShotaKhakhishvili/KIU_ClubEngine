#include <Engine/Input/FActionKeyMapping.h>

#include <algorithm>

namespace CE
{

FActionKeyMapping::FActionKeyMapping(const EKey key, UInputAction* inputAction)
        : key(key), inputAction(inputAction)
{
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

const TArray<std::unique_ptr<UInputModifier>>& FActionKeyMapping::GetModifiers() const
{
    return modifiers;
}

const TArray<std::unique_ptr<UInputTrigger>>& FActionKeyMapping::GetTriggers() const
{
    return triggers;
}

}