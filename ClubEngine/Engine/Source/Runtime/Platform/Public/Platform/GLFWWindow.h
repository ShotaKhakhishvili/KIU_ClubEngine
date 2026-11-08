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
    ~GLFWWindow                 ()                                      override;
    bool    Init                (const FWindowConfig& config)           override;
    void    Shutdown            ()                                      override;
    void    PollEvents          ()                                      override;
    void    SwapBuffers         ()                                      override;

    bool    ShouldClose         ()                        const         override;
    uint32  GetWidth            ()                        const         override;
    uint32  GetHeight           ()                        const         override;
    float   GetAspectRatio      ()                        const         override;
    void*   GetNativeHandle     ()                        const         override;

    void    ConsumeInputEvents  (TArray<FInputEvent>& outEvents)        override;
private:
    void    InitGL              (const FWindowConfig& config);
    void    InitNoAPI           (const FWindowConfig& config);

    GLFWwindow*             handle              = nullptr;
    uint32                  height              = 0;
    uint32                  width               = 0;
    RHI::Backend            backend             = RHI::Backend::OpenGL;

    void InstallInputCallbacks();

    TArray<FInputEvent>     pendingEvents;
    FVector2F               lastCursorPos       = FVector2F(0, 0);
    bool                    hasCursorPos        = false;
};

}
