#pragma once

#include <Engine/ISystem.h>
#include <Engine/Input/Types/FInputState.h>
namespace CE
{

class InputSystem : public ISystem
{
public:

    const char* GetName() const noexcept override;
    bool Init() override;
    int Update(float dt) override;

private:
    FInputState inputState{};
};

}
