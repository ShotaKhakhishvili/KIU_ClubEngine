#pragma once

#include <Platform/IWindow.h>
#include <Platform/WindowConfig.h>

#include <Engine/ISystem.h>

#include <memory>


namespace CE
{

class WindowSystem : public ISystem
{
public:
    explicit WindowSystem(WindowConfig config = {});

    const char*     GetName         ()              const       noexcept        override;
    bool            Initialize      ()                                          override;
    void            Shutdown        ()                                          override;
    int             Update          (float dt)                                  override;

    IWindow*        GetWindow       ()              const;
    bool            ShouldClose     ()              const;

private:
    WindowConfig                    config;
    std::unique_ptr<IWindow>        window;
};

}