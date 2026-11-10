#include <Engine/Input/FPlayerInput.h>

#include <Engine/Input/Types/FInputState.h>
#include <Engine/Input/UInputMapContext.h>
#include <Engine/Input/UInputComponent.h>

#include <algorithm>
#include <bitset>
#include <cmath>

namespace CE
{

void FPlayerInput::AddMappingContext(const UInputMapContext* context, const int32 priority)
{
    if (!context)
        return;

    RemoveMappingContext(context);

    const auto position = std::ranges::find_if(contexts, [priority](const FContextEntry& entry)
    {
        return entry.priority < priority;
    });
    contexts.insert(position, {context, priority});
}

void FPlayerInput::RemoveMappingContext(const UInputMapContext* context)
{
    std::erase_if(contexts, [context](const FContextEntry& entry)
    {
        return entry.context == context;
    });
}

void FPlayerInput::ClearMappingContexts()
{
    contexts.clear();
}

void FPlayerInput::Process(const FInputState& state, const float dt, const TArray<UInputComponent*>& inputStack)
{
    firedEvents.clear();
    RebuildActiveMappings();

    const auto& transitions = state.pressReleaseList;

    std::array<int32, EKey::Count> lastTransition{};
    lastTransition.fill(-1);
    for (size_t i = 0; i < transitions.size(); i++)
        lastTransition[transitions[i].key] = static_cast<int32>(i);

    TArray<UInputAction*> touchedActions;
    for (size_t i = 0; i < transitions.size(); i++)
    {
        if (lastTransition[transitions[i].key] == static_cast<int32>(i))
            continue;

        touchedActions.clear();
        for (const int32 index : mappingsByKey[transitions[i].key])
        {
            FActiveMapping& active = activeMappings[index];
            EvaluateMapping(active, transitions[i].pressed ? 1.0f : 0.0f, 0.0f);

            if (std::ranges::find(touchedActions, active.action) == touchedActions.end())
                touchedActions.push_back(active.action);
        }

        for (UInputAction* action : touchedActions)
            EvaluateAction(action, 0.0f, false);
    }

    for (FActiveMapping& active : activeMappings)
        EvaluateMapping(active, state.GetKeyValue(active.key), dt);

    for (const auto& [action, indices] : mappingsByAction)
        EvaluateAction(action, dt, true);

    EndRemovedActions();
    Dispatch(inputStack);
}

void FPlayerInput::EvaluateMapping(FActiveMapping& active, const float keyValue, const float dt)
{
    FInputActionValue value{};
    value.value.x = keyValue;
    value.type = active.action->GetValueType();

    for (const std::unique_ptr<UInputModifier>& modifier : active.mapping->GetModifiers())
        value = modifier->ModifyRaw(value, dt);

    ZeroUnusedAxes(value);

    const auto& triggers = active.mapping->GetTriggers();
    if (triggers.empty())
        active.state = LengthSq(value.value) > 0.0f ? ETriggerState::Triggered : ETriggerState::None;
    else
        active.state = EvaluateTriggers(triggers, value, dt);

    active.value = value;
}

void FPlayerInput::EvaluateAction(UInputAction* action, const float dt, const bool finalPass)
{
    FVectorF combined{};
    ETriggerState mappingState = ETriggerState::None;

    if (const auto it = mappingsByAction.find(action); it != mappingsByAction.end())
    {
        for (const int32 index : it->second)
        {
            const FActiveMapping& active = activeMappings[index];

            if (std::abs(active.value.value.x) > std::abs(combined.x)) combined.x = active.value.value.x;
            if (std::abs(active.value.value.y) > std::abs(combined.y)) combined.y = active.value.value.y;
            if (std::abs(active.value.value.z) > std::abs(combined.z)) combined.z = active.value.value.z;

            mappingState = std::max(mappingState, active.state);
        }
    }

    FInputActionValue value{combined, action->GetValueType()};

    for (const std::unique_ptr<UInputModifier>& modifier : action->GetModifiers())
        value = modifier->ModifyRaw(value, dt);

    ZeroUnusedAxes(value);

    ETriggerState state = mappingState;
    if (const auto& triggers = action->GetTriggers(); !triggers.empty())
        state = std::min(mappingState, EvaluateTriggers(triggers, value, dt));

    FActionState& actionState = actionStates[action];
    EmitEvents(action, actionState.state, state, value, finalPass);
    actionState.state = state;
    actionState.value = value;
}

void FPlayerInput::RebuildActiveMappings()
{
    std::swap(previousMappings, activeMappings);
    activeMappings.clear();
    for (auto& indices : mappingsByKey)
        indices.clear();
    mappingsByAction.clear();

    std::bitset<EKey::Count> claimedKeys;
    for (const FContextEntry& entry : contexts)
    {
        std::bitset<EKey::Count> contextKeys;

        for (const FActionKeyMapping& mapping : entry.context->GetMappings())
        {
            const EKey key = mapping.GetKey();
            UInputAction* action = mapping.GetInputAction();

            if (!action || key == EKey::None || claimedKeys.test(key))
                continue;

            contextKeys.set(key);

            FActiveMapping active{entry.context, &mapping, action, key};

            // Keep the last result, so a replay step can combine with the mappings it doesn't re-run
            for (const FActiveMapping& previous : previousMappings)
            {
                if (previous.context == entry.context && previous.action == action && previous.key == key)
                {
                    active.value = previous.value;
                    active.state = previous.state;
                    break;
                }
            }

            const auto index = static_cast<int32>(activeMappings.size());
            activeMappings.push_back(active);
            mappingsByKey[key].push_back(index);
            mappingsByAction[action].push_back(index);
        }

        claimedKeys |= contextKeys;
    }
}

ETriggerState FPlayerInput::EvaluateTriggers(const TArray<std::unique_ptr<UInputTrigger>>& triggers, const FInputActionValue& value, const float dt)
{
    ETriggerState state = ETriggerState::None;
    for (const std::unique_ptr<UInputTrigger>& trigger : triggers)
        state = std::max(state, trigger->Update(value, dt));

    return state;
}

void FPlayerInput::EmitEvents(UInputAction* action, const ETriggerState last, const ETriggerState now,
                              const FInputActionValue& value, const bool finalPass)
{
    if (!finalPass && last == now)
        return;

    auto fire = [&](const ETriggerEvent event)
    {
        firedEvents.push_back({action, event, value});
    };

    switch (now)
    {
    case ETriggerState::None:
        if (last == ETriggerState::Ongoing)     fire(ETriggerEvent::Canceled);
        if (last == ETriggerState::Triggered)   fire(ETriggerEvent::Completed);
        break;

    case ETriggerState::Ongoing:
        if (last == ETriggerState::None)        fire(ETriggerEvent::Started);
        fire(ETriggerEvent::Ongoing);
        break;

    case ETriggerState::Triggered:
        if (last == ETriggerState::None)        fire(ETriggerEvent::Started);
        fire(ETriggerEvent::Triggered);
        break;
    }
}

void FPlayerInput::EndRemovedActions()
{
    for (auto it = actionStates.begin(); it != actionStates.end();)
    {
        if (mappingsByAction.contains(it->first))
        {
            ++it;
            continue;
        }

        const FInputActionValue zero{FVectorF{}, it->second.value.type};
        EmitEvents(it->first, it->second.state, ETriggerState::None, zero, true);
        it = actionStates.erase(it);
    }
}

void FPlayerInput::Dispatch(const TArray<UInputComponent*>& inputStack) const
{
    const TArray<UInputComponent*> stack = inputStack;

    for (const FFiredEvent& fired : firedEvents)
    {
        for (UInputComponent* component : stack)
        {
            if (component)
                component->DispatchActionTrigger(fired.action, fired.event, fired.value);
        }
    }
}

void FPlayerInput::ZeroUnusedAxes(FInputActionValue& value)
{
    switch (value.type)
    {
    case EInputActionValueType::Bool:
        value.value = {LengthSq(value.value) > 0.0f ? 1.0f : 0.0f, 0.0f, 0.0f};
        break;
    case EInputActionValueType::Axis1D:
        value.value.y = 0.0f;
        value.value.z = 0.0f;
        break;
    case EInputActionValueType::Axis2D:
        value.value.z = 0.0f;
        break;
    case EInputActionValueType::Axis3D:
        break;
    }
}

}