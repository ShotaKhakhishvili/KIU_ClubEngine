#pragma once

namespace CE
{

template<typename T>
FDelegateHandle UInputComponent::BindAction(UInputAction* action, const ETriggerEvent triggerEvent, T* context,
    void (T::*function)(const FInputActionValue&))
{
    if (!action || !context || !function)
        return {};

    return bindings[action][triggerEvent].Add(context, function);
}

template <typename T>
FDelegateHandle UInputComponent::BindAction(UInputAction* action, ETriggerEvent triggerEvent, T* context, void (T::*function)(const FInputActionValue&) const)
{
    if (!action || !context || !function)
        return {};

    return bindings[action][triggerEvent].Add(context, function);
}

}