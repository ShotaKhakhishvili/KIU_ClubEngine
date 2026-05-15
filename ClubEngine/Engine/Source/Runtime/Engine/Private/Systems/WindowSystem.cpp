#include <Engine/Systems/WindowSystem.h>

namespace CE
{

WindowSystem::WindowSystem(WindowConfig config)
    : config(config)
{
}

bool WindowSystem::Initialize()
{
    window = IWindow::Create();

    if (!window->Init(config))
    {
        CE_LOG(Error, "WindowSystem: IWindow::Initialize failed");
        return false;
    }

    CE_LOG(Info, "WindowSystem: Window created ({}x{})", config.width, config.height);
    return true;
}

int WindowSystem::Update(float dt)
{
    (void)dt;
    window->PollEvents();
    return window->ShouldClose() ? -1 : 0;
}

void WindowSystem::Shutdown()
{
    window->Shutdown();
}

const char* WindowSystem::GetName() const noexcept      {   return "WindowSystem";                          }
IWindow* WindowSystem::GetWindow() const                {   return window.get();                            }
bool WindowSystem::ShouldClose() const                  {   return window && window->ShouldClose();     }

}