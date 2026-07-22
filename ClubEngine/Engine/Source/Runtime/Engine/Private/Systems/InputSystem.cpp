#include <Engine/Systems/InputSystem.h>
#include <Engine/Systems/WindowSystem.h>
#include <Platform/IWindow.h>
#include <Core/ClubCore.h>

namespace CE
{
    InputSystem::InputSystem(WindowSystem* windowSystem)
        : m_WindowSystem(windowSystem)
    {
    }

    const char* InputSystem::GetName() const noexcept
    {
        return "InputSystem";
    }

    bool InputSystem::Init()
    {
        if (!m_WindowSystem) return false;

        IWindow* window = m_WindowSystem->GetWindow();
        if (!window) return false;

        window->OnKey.Add([this](EKeyCode key, EInputAction action) {
            this->HandleKeyEvent(key, action);
        });

        window->OnMouseButton.Add([this](EMouseButton button, EInputAction action) {
            this->HandleMouseEvent(button, action);
        });

        window->OnMouseMove.Add([this](float x, float y) {
            this->HandleMouseMove(x, y);
        });

        return true;
    }

    int InputSystem::Update(float)
    {
        return 0;
    }

    void InputSystem::Shutdown()
    {
    }

    bool InputSystem::IsKeyPressed(EKeyCode key) const
    {
        auto it = m_KeyStates.find(key);
        return it != m_KeyStates.end() ? it->second : false;
    }

    bool InputSystem::IsMouseButtonPressed(EMouseButton button) const
    {
        auto it = m_MouseStates.find(button);
        return it != m_MouseStates.end() ? it->second : false;
    }

    void InputSystem::HandleKeyEvent(EKeyCode key, EInputAction action)
    {
        if (action == EInputAction::Press)
        {
            m_KeyStates[key] = true;

            CE_LOG(Info, "Key Pressed! Code: {}", static_cast<int>(key)); 
        }
        else if (action == EInputAction::Release)
        {
            m_KeyStates[key] = false;

            CE_LOG(Info, "Key Released! Code: {}", static_cast<int>(key));
        }
    }

    void InputSystem::HandleMouseEvent(EMouseButton button, EInputAction action)
    {
        if (action == EInputAction::Press){
            m_MouseStates[button] = true;

            CE_LOG(Info, "Mouse Button Clicked! Button: {}", static_cast<int>(button));
        }
        else if (action == EInputAction::Release){
            m_MouseStates[button] = false;
        }

    }

    void InputSystem::HandleMouseMove(float x, float y)
    {
        m_MouseX = x;
        m_MouseY = y;
    }
}