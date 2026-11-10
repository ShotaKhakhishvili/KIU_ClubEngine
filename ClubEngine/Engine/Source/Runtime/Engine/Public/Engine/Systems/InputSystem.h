#pragma once

#include <Core/Containers/TArray.h>

#include <Engine/ISystem.h>
#include <Engine/Input/Types/FInputState.h>
#include <Engine/Input/FPlayerInput.h>

namespace CE
{
class WindowSystem;

class InputSystem : public ISystem
{
public:
    explicit InputSystem(WindowSystem* windowSystem);

    const char* GetName() const noexcept override;
    bool Init() override;
    int Update(float dt) override;

    const FInputState& GetState() const;

    FPlayerInput testPlayerInput;
    UInputComponent* testInputComponent = nullptr;

private:
    WindowSystem* windowSystem;

    FInputState inputState{};
    TArray<FInputEvent> frameEvents;
};

}
