namespace CE
{

template<typename T>
FDelegateHandle UInputComponent::BindAction(UInputAction* action, const ETriggerEvent triggerEvent, UObject* context,
    void (T::*function)(const FInputActionValue&))
{
    if (!action || context || !function)
        return {};

    return bindings[action][triggerEvent].Add(context, function);
}

}