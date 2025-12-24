#include "ClubEngine.h"

int WINDOW_WIDTH = 1600, WINDOW_HEIGHT = 900;
Font* FONT_MAIN  = nullptr;

GLFWwindow* CreateWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 1);

    // Actually create the window and return it
    return glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL_Proj1 Window", nullptr, nullptr);
}

int main() {
    glfwInit();

    GLFWwindow* window = CreateWindow();

    // Check if the window was created successfully
    if (window == NULL)
    {
        // Terminate if it failed to create the window
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "Successfully created" << std::endl;

    glfwMakeContextCurrent(window); // Actually display the window
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }


    // -------------------------------General Settings----------------------------------------

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // -----------------------------------Camera--------------------------------------------------

    Camera::Init(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.0f, 0.0f, 1.0f));

    // ----------------------------------TextRenderer----------------------------------------------

    FONT_MAIN = new Font(Path::Font("RobotoMono-Regular.ttf"), 48);

    // -----------------------------------Initialization-------------------------------------

    World::worldTime = glfwGetTime();
    Game::Init();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Camera::camera->ProccessInputs(window);

        World::Update();
        World::Draw();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}