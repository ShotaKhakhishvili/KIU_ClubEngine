#pragma once

#include <Engine/Input/UInputModifier.h>
#include <Engine/Input/UInputTrigger.h>

#include <CoreUObject/UObject.h>

#include <memory>
#include <type_traits>

namespace CE
{

class UInputAction : public UObject
{
    GENERATED_BODY(UInputAction, UObject)

public:
    UInputAction(EInputActionValueType valueType);

    template<typename T, typename... Args>
        requires std::is_base_of_v<UInputTrigger, T>
    T* AddTrigger(Args&&... args);

    template<typename T, typename... Args>
        requires std::is_base_of_v<UInputModifier, T>
    T* AddModifier(Args&&... args);

    EInputActionValueType GetValueType() const;

    const TArray<std::unique_ptr<UInputTrigger>>& GetTriggers() const;
    const TArray<std::unique_ptr<UInputModifier>>& GetModifiers() const;

private:
    EInputActionValueType valueType;
    TArray<std::unique_ptr<UInputModifier>> modifiers;
    TArray<std::unique_ptr<UInputTrigger>> triggers;
};

}


#include "UInputAction.inl"