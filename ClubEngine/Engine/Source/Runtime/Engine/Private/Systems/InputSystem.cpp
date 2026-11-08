#include <Engine/Systems/InputSystem.h>

#include <Engine/Systems/WindowSystem.h>

namespace CE
{

InputSystem::InputSystem(WindowSystem* windowSystem)
    : windowSystem(windowSystem)
{
}
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

    IWindow* window = windowSystem->GetWindow();
    if (!window)
        return -1;

    window->ConsumeInputEvents(frameEvents);

    inputState.pressed.reset();
    inputState.released.reset();
    inputState.mouseDelta = {};
    inputState.wheelDelta = 0.0f;

    for (const auto& event : frameEvents)
    {
        switch (event.type)
        {
        case EInputEventType::KeyDown:
            inputState.pressed.set(event.key);
            inputState.down.set(event.key);
            break;

        case EInputEventType::KeyUp:
            inputState.released.set(event.key);
            inputState.down.reset(event.key);
            break;

        case EInputEventType::MouseMove:
            inputState.mouseDelta.x += event.x;
            inputState.mouseDelta.y += event.y;
            break;

        case EInputEventType::MouseWheel:
            inputState.wheelDelta += event.y;
            break;

        case EInputEventType::FocusLost:
            inputState.released |= inputState.down;
            inputState.down.reset();
            break;
        }
    }

    return 0;
}
const FInputState& InputSystem::GetState() const
{
    return inputState;
}
}