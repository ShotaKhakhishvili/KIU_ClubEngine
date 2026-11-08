#include <Core/ClubCore.h>

#include <Platform/GLFWWindow.h>

#include "GLFWConvert.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <memory>

namespace CE
{
std::unique_ptr<IWindow> IWindow::Create()
{
    return std::make_unique<GLFWWindow>();
}

GLFWWindow::~GLFWWindow()
{
    GLFWWindow::Shutdown();
}

bool GLFWWindow::Init(const FWindowConfig& config)
{
    if(!glfwInit())
    {
        CE_LOG(Error, "Failed To Initialise GLFW!");
        return false;
    }

    this->width = config.width;
    this->height = config.height;
    this->backend = config.backend;

    switch(config.backend)
    {
        case RHI::Backend::OpenGL :
        {
            InitGL(config);
            break;
        }
    }

    if(!handle)
    {
        CE_LOG(Error, "GLFW Window Creation Failed!");
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(handle, this);

    glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* win, int W, int H)
    {
        auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(win));
        self->width = static_cast<uint32>(W);
        self->height = static_cast<uint32>(H);
        CE_LOG(Info, "width: {}, height: {}", W, H);
    });

    InstallInputCallbacks();

    return true;
}

void GLFWWindow::Shutdown()
{    
    if (handle)
    {
        glfwDestroyWindow(handle);
        handle = nullptr;
        glfwTerminate();
    }
}

void GLFWWindow::PollEvents()
{
    glfwPollEvents();
}

void GLFWWindow::SwapBuffers()
{
    if (backend == RHI::Backend::OpenGL)
        glfwSwapBuffers(handle);
}

bool GLFWWindow::ShouldClose() const
{
    return glfwWindowShouldClose(handle);
}

float   GLFWWindow::GetAspectRatio  () const   
{   
    return height > 0 ? 
         static_cast<float>(width) / static_cast<float>(height) : 0;                           
}

uint32  GLFWWindow::GetWidth            ()      const   {   return width;                           }
uint32  GLFWWindow::GetHeight           ()      const   {   return height;                          }
void*   GLFWWindow::GetNativeHandle     ()      const   {   return handle;                          }

void GLFWWindow::ConsumeInputEvents(TArray<FInputEvent>& outEvents)
{
    std::swap(outEvents, pendingEvents);
    pendingEvents.clear();
}

void GLFWWindow::InitGL(const FWindowConfig& config)
{
    glfwWindowHint(GLFW_CLIENT_API,   GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(
        static_cast<int>(config.width),
        static_cast<int>(config.height),
        config.title.c_str(),
        nullptr, nullptr
    );

    if (handle)
    {
        glfwMakeContextCurrent(handle);
        gladLoadGL();
        glfwSwapInterval(config.vSync ? 1 : 0);
    }
}

void GLFWWindow::InitNoAPI(const FWindowConfig& config)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    handle = glfwCreateWindow(
        static_cast<int>(config.width),
        static_cast<int>(config.height),
        config.title.c_str(),
        nullptr, nullptr
    );
}
void GLFWWindow::InstallInputCallbacks()
{
    glfwSetKeyCallback(handle, [](GLFWwindow* win, const int key, const int scancode, const int action, const int mods)
    {
        (void)scancode;
        (void)mods;

        if (action == GLFW_REPEAT)
            return;

        EKey eKey = ToEKey(key);
        if (eKey == EKey::None)
            return;

        const auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(win));
        self->pendingEvents.push_back({action == GLFW_PRESS ? EInputEventType::KeyDown : EInputEventType::KeyUp, eKey});
    });

    glfwSetMouseButtonCallback(handle, [](GLFWwindow* win, const int button, const int action, const int mods)
    {
        (void)mods;

        const EKey engineKey = ToEKeyMouse(button);
        if (engineKey == EKey::None)
            return;

        auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(win));
        self->pendingEvents.push_back({ action == GLFW_PRESS ? EInputEventType::KeyDown : EInputEventType::KeyUp, engineKey });
    });

    glfwSetCursorPosCallback(handle, [](GLFWwindow* win, double x, double y)
    {
        auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(win));

        if (self->hasCursorPos)
        {
            self->pendingEvents.push_back({ EInputEventType::MouseMove, EKey::None,
                static_cast<float>(x - self->lastCursorPos.x), static_cast<float>(y - self->lastCursorPos.y) });
        }

        self->lastCursorPos = {x,y};
        self->hasCursorPos = true;
    });

    glfwSetScrollCallback(handle, [](GLFWwindow* win, const double xOffset, const double yOffset)
    {
        (void)xOffset;

        auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(win));
        self->pendingEvents.push_back({ EInputEventType::MouseWheel, EKey::None, 0.0f, static_cast<float>(yOffset) });
    });

    glfwSetWindowFocusCallback(handle, [](GLFWwindow* win, int focused)
    {
        auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(win));
        self->hasCursorPos = false;

        if (!focused)
            self->pendingEvents.push_back({ EInputEventType::FocusLost });
    });
}
}