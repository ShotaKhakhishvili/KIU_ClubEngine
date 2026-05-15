#pragma once

#include <Core/ClubCore.h>
#include <Core/Types/RHITypes.h>

#include <Platform/IWindow.h>

struct GLFWwindow;

namespace CE
{

class GLFWWindow : public IWindow
{
public:
    bool    Init                (const WindowConfig& config)    override;
    void    Shutdown            ()                              override;
    void    PollEvents          ()                              override;
    void    SwapBuffers         ()                              override;

    bool    ShouldClose         ()                        const override;
    uint32  GetWidth            ()                        const override;
    uint32  GetHeight           ()                        const override;
    float   GetAspectRatio      ()                        const override;
    void*   GetNativeHandle     ()                        const override;

private:
    void    InitGL              (const WindowConfig& config);
    void    InitNoAPI           (const WindowConfig& config);

    GLFWwindow*     handle          = nullptr;
    uint32          height          = 0;
    uint32          width           = 0;
    RHI::Backend    backend         = RHI::Backend::OpenGL;
};

}
