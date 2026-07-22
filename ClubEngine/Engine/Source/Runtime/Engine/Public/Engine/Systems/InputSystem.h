#pragma once
#include <Engine/ISystem.h>
#include <Core/Types/InputTypes.h>
#include <unordered_map>

namespace CE
{
    class WindowSystem;

    class InputSystem : public ISystem
    {
    public:
        InputSystem(WindowSystem* windowSystem);
        virtual ~InputSystem() = default;

        virtual const char* GetName() const noexcept override;
        virtual bool Init() override;
        virtual int Update(float dt) override;
        virtual void Shutdown() override;

        bool IsKeyPressed(EKeyCode key) const;
        bool IsMouseButtonPressed(EMouseButton button) const;
        float GetMouseX() const { return m_MouseX; }
        float GetMouseY() const { return m_MouseY; }

    private:
        void HandleKeyEvent(EKeyCode key, EInputAction action);
        void HandleMouseEvent(EMouseButton button, EInputAction action);
        void HandleMouseMove(float x, float y);

        WindowSystem* m_WindowSystem;
        
        std::unordered_map<EKeyCode, bool> m_KeyStates;
        std::unordered_map<EMouseButton, bool> m_MouseStates;
        float m_MouseX = 0.0f;
        float m_MouseY = 0.0f;
    };
}