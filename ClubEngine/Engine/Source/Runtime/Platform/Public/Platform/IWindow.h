#pragma once

#include <Core/ClubCore.h>

#include <Platform/WindowConfig.h>

#include <memory>

namespace CE
{

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

    static std::unique_ptr<IWindow> Create();
};

}