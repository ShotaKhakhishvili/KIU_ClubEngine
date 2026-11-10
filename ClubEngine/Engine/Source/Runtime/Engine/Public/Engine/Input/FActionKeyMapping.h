#pragma once

#include <Core/Types/InputTypes.h>

#include <Engine/Input/UInputTrigger.h>
#include <Engine/Input/UInputModifier.h>
#include <Engine/Input/UInputAction.h>

#include <memory>
#include <type_traits>

namespace CE
{
class FActionKeyMapping
{
public:
    FActionKeyMapping(EKey key, UInputAction* inputAction);

    void SetKey(EKey key);
    EKey GetKey() const;

    void SetInputAction(UInputAction* inputAction);
    UInputAction* GetInputAction() const;

    template<typename T, typename... Args>
        requires std::is_base_of_v<UInputTrigger, T>
    T* AddTrigger(Args&&... args);

    template<typename T, typename... Args>
        requires std::is_base_of_v<UInputModifier, T>
    T* AddModifier(Args&&... args);

    const TArray<std::unique_ptr<UInputModifier>>& GetModifiers() const;
    const TArray<std::unique_ptr<UInputTrigger>>& GetTriggers() const;

private:
    EKey key;

    UInputAction* inputAction{};
    TArray<std::unique_ptr<UInputTrigger>> triggers;
    TArray<std::unique_ptr<UInputModifier>> modifiers;
};
}

#include "FActionKeyMapping.inl"
