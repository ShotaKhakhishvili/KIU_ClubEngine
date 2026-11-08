#pragma once

#include <Core/Delegate/TMulticastDelegate.h>
#include <Core/Containers/TMap.h>

#include <Engine/World/UActorComponent.h>
#include <Engine/Input/UInputAction.h>
#include <Engine/Input/Types/ETriggerEvent.h>

namespace CE
{

class UInputComponent : public UActorComponent
{
    GENERATED_BODY(UInputComponent, UActorComponent)

public:
    using UActorComponent::UActorComponent;

    template<typename T>
    FDelegateHandle BindAction(UInputAction* action, ETriggerEvent triggerEvent, T* context,
                void (T::*function)(const FInputActionValue&));

    template<typename T>
    FDelegateHandle BindAction(UInputAction* action, ETriggerEvent triggerEvent, T* context,
            void (T::*function)(const FInputActionValue&) const);

    FDelegateHandle BindAction(UInputAction* action, ETriggerEvent triggerEvent,
            const std::function<void(const FInputActionValue&)>& callback);

    void DispatchActionTrigger(UInputAction* action, ETriggerEvent triggerEvent, const FInputActionValue& actionValue);

private:
    TMap<UInputAction*, TMap<ETriggerEvent, TMulticastDelegate<const FInputActionValue&>>> bindings;
};

}

#include "UInputComponent.inl"
