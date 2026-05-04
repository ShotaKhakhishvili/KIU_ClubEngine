#include <Core/Input.h>
#include <Core/ClubCore.h>
#include <GLFW/glfw3.h>

namespace CE
{
    static GLFWwindow* s_DummyWindow = nullptr;

    void Input::InitDummyWindow()
    {
        if (!glfwInit())
        {
            return;
        }

        s_DummyWindow = glfwCreateWindow(640, 480, "Dummy Input Window", nullptr, nullptr);
        
        if (!s_DummyWindow)
        {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(s_DummyWindow);
        CE_LOG(Info, "Dummy Window Created");
    }

    void Input::UpdateDummyWindow()
    {
        if (s_DummyWindow)
        {
            glfwPollEvents(); 
        }
    }

    bool Input::IsKeyPressed(KeyCode key)
    {
        GLFWwindow* window = s_DummyWindow ? s_DummyWindow : glfwGetCurrentContext();
        
        if (!window) return false;

        int state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        GLFWwindow* window = s_DummyWindow ? s_DummyWindow : glfwGetCurrentContext();
        
        if (!window) return false;

        int state = glfwGetMouseButton(window, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    void Input::GetMousePosition(float& outX, float& outY)
    {
        GLFWwindow* window = s_DummyWindow ? s_DummyWindow : glfwGetCurrentContext();
        
        if (!window) 
        {
            outX = 0.0f;
            outY = 0.0f;
            return;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        outX = static_cast<float>(xpos);
        outY = static_cast<float>(ypos);
    }

    float Input::GetMouseX()
    {
        float x, y;
        GetMousePosition(x, y);
        return x;
    }

    float Input::GetMouseY()
    {
        float x, y;
        GetMousePosition(x, y);
        return y;
    }
}