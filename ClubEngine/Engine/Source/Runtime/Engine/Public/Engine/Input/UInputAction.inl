#pragma once

namespace CE
{

template<typename T, typename... Args>
    requires std::is_base_of_v<UInputTrigger, T>
T* UInputAction::AddTrigger(Args&&... args)
{
    triggers.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    return static_cast<T*>(triggers.back().get());
}

template<typename T, typename... Args>
    requires std::is_base_of_v<UInputModifier, T>
T* UInputAction::AddModifier(Args&&... args)
{
    modifiers.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    return static_cast<T*>(modifiers.back().get());
}

}