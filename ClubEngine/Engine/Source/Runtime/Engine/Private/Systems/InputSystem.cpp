#include <Engine/Systems/InputSystem.h>

namespace CE
{

const char* InputSystem::GetName() const noexcept
{
    return "InputSystem";
}
bool InputSystem::Init()
{
    return true;
}
int InputSystem::Update(const float dt)
{
    (void)dt;
    return 0;
}
}