#pragma once

#include <Core/Containers/TArray.h>
#include <Core/Containers/TMap.h>
#include <Core/Types/InputTypes.h>

#include <Engine/Input/Types/FInputActionValue.h>
#include <Engine/Input/Types/ETriggerState.h>
#include <Engine/Input/Types/ETriggerEvent.h>

#include <array>
#include <memory>

namespace CE
{

struct FInputState;
class FActionKeyMapping;
class UInputMapContext;
class UInputAction;
class UInputComponent;
class UInputTrigger;

class FPlayerInput
{
public:
    void AddMappingContext      (const UInputMapContext* context, int32 priority = 0);
    void RemoveMappingContext   (const UInputMapContext* context);
    void ClearMappingContexts   ();

    void Process                (const FInputState& state, float dt, const TArray<UInputComponent*>& inputStack);

private:
    struct FContextEntry
    {
        const UInputMapContext*     context{};
        int32                       priority{};
    };

    struct FActiveMapping
    {
        const UInputMapContext*     context{};
        const FActionKeyMapping*    mapping{};
        UInputAction*               action;
        EKey                        key;
        FInputActionValue           value;
        ETriggerState               state = ETriggerState::None;
    };

    struct FActionState
    {
        ETriggerState               state = ETriggerState::None;
        FInputActionValue           value{};
    };

    struct FFiredEvent
    {
        UInputAction*               action;
        ETriggerEvent               event;
        FInputActionValue           value;
    };

    void RebuildActiveMappings();

    void                    EvaluateMapping         (FActiveMapping& active, float keyValue, float dt);
    void                    EvaluateAction          (UInputAction* action, float dt, bool finalPass);
    void                    EmitEvents              (UInputAction* action, ETriggerState last, ETriggerState now,
                                                     const FInputActionValue& value, bool finalPass);
    void                    EndRemovedActions       ();
    void                    Dispatch                (const TArray<UInputComponent*>& inputStack) const;

    static ETriggerState    EvaluateTriggers        (const TArray<std::unique_ptr<UInputTrigger>>& triggers, const FInputActionValue& value, float dt);
    static void             ZeroUnusedAxes          (FInputActionValue& value);

    TArray<FContextEntry>                   contexts;

    TArray<FActiveMapping>                  activeMappings;
    TArray<FActiveMapping>                  previousMappings;
    std::array<TArray<int32>, EKey::Count>  mappingsByKey;
    TMap<UInputAction*, TArray<int32>>      mappingsByAction;

    TMap<UInputAction*, FActionState>       actionStates;
    TArray<FFiredEvent>                     firedEvents;
};

}