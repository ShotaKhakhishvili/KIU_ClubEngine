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
void UInputComponent::DispatchActionTrigger(UInputAction* action, ETriggerEvent triggerEvent, const FInputActionValue& actionValue)
{
    const auto triggerMap = bindings.find(action);
    if (triggerMap == bindings.end())
        return;

    auto triggerDispatcher = triggerMap->second.find(triggerEvent);
    if (triggerDispatcher != triggerMap->second.end())
        triggerDispatcher->second.Broadcast(actionValue);
}
}