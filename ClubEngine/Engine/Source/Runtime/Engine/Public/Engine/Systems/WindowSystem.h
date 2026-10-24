#pragma once

#include <Platform/IWindow.h>
#include <Platform/FWindowConfig.h>

#include <Engine/ISystem.h>

#include <memory>


namespace CE
{

class WindowSystem : public ISystem
{
public:
    explicit WindowSystem(FWindowConfig config = {});

    const char*     GetName         ()              const       noexcept        override;
    bool            Init            ()                                          override;
    void            Shutdown        ()                                          override;
    int             Update          (float dt)                                  override;

    IWindow*        GetWindow       ()              const;
    bool            ShouldClose     ()              const;

private:
    FWindowConfig                    config;
    std::unique_ptr<IWindow>        window;
};

}