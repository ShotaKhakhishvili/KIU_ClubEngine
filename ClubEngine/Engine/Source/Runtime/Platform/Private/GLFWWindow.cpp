#include <Core/ClubCore.h>

#include <Platform/GLFWWindow.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <memory>

namespace CE
{
std::unique_ptr<IWindow> IWindow::Create()
{
    return std::make_unique<GLFWWindow>();
}

bool GLFWWindow::Init(const WindowConfig& config)
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
    });

    return true;
}

void GLFWWindow::Shutdown()
{    
    if (handle)
    {
        glfwDestroyWindow(handle);
        handle = nullptr;
    }
    glfwTerminate();
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

void GLFWWindow::InitGL(const WindowConfig& config)
{
    glfwWindowHint(GLFW_CLIENT_API,   GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

void GLFWWindow::InitNoAPI(const WindowConfig& config)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    handle = glfwCreateWindow(
        static_cast<int>(config.width),
        static_cast<int>(config.height),
        config.title.c_str(),
        nullptr, nullptr
    );
}
}