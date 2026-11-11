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

    inputState.pressReleaseList.clear();
    inputState.mouseDelta = {};
    inputState.wheelDelta = 0.0f;

    for (const auto& event : frameEvents)
    {
        switch (event.type)
        {
        case EInputEventType::KeyDown:
            inputState.down.set(event.key);
            inputState.pressReleaseList.emplace_back(event.key, true);
            break;

        case EInputEventType::KeyUp:
            inputState.down.reset(event.key);
            inputState.pressReleaseList.emplace_back(event.key, false);
            break;

        case EInputEventType::MouseMove:
            inputState.mouseDelta.x += event.x;
            inputState.mouseDelta.y += event.y;
            break;

        case EInputEventType::MouseWheel:
            inputState.wheelDelta += event.y;
            break;

        case EInputEventType::FocusLost:
            for (size_t i = 0; i < inputState.down.size(); ++i)
            {
                if (inputState.down.test(i))
                    inputState.pressReleaseList.emplace_back(static_cast<EKey>(i), false);
            }
            inputState.down.reset();
            break;
        }
    }

    inputState.mouseDelta = window->GetCursorPosition();

    return 0;
}
const FInputState& InputSystem::GetState() const
{
    return inputState;
}
}