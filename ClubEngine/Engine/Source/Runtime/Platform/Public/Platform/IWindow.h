#pragma once

#include <Core/ClubCore.h>
#include <Core/Delegate/MulticastDelegate.h>
#include <Core/Types/InputTypes.h>

#include <Platform/WindowConfig.h>

#include <memory>

namespace CE
{

using FOnKeyEvent = MulticastDelegate<EKeyCode, EInputAction>;
using FOnMouseEvent = MulticastDelegate<EMouseButton, EInputAction>;
using FOnMouseMoveEvent = MulticastDelegate<float, float>;

class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual bool Init(const WindowConfig& config)   = 0;
    virtual void Shutdown()                         = 0;
    virtual void PollEvents()                       = 0;
    virtual void SwapBuffers()                      = 0;

    virtual bool ShouldClose()                const = 0;
    virtual uint32 GetWidth()                 const = 0;
    virtual uint32 GetHeight()                const = 0;
    virtual float GetAspectRatio()            const = 0;
    virtual void* GetNativeHandle()           const = 0;

    FOnKeyEvent OnKey;
    FOnMouseEvent OnMouseButton;
    FOnMouseMoveEvent OnMouseMove;

    static std::unique_ptr<IWindow> Create();
};

}