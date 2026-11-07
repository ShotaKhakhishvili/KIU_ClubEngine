#include <Engine/Input/UInputComponent.h>

namespace CE
{

FDelegateHandle UInputComponent::BindAction(UInputAction* action, ETriggerEvent triggerEvent,
    const std::function<void(const FInputActionValue&)>& callback)
{
    if (!action || !callback)
        return {};

    return bindings[action][triggerEvent].Add(callback);
}
}