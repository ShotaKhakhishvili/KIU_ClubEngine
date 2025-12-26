#include "ClubEngine.h"

// ------------------------------------------------------------
// Globals (engine-style, fine for now)
// ------------------------------------------------------------
int WINDOW_WIDTH  = 1600;
int WINDOW_HEIGHT = 900;

Font*       FONT_MAIN    = nullptr;
GLFWwindow* GAME_WINDOW  = nullptr;

// ------------------------------------------------------------
// Framebuffer resize callback (CRITICAL)
// ------------------------------------------------------------
static void FramebufferSizeCallback(GLFWwindow*, int width, int height)
{
    if (height == 0) height = 1;

    WINDOW_WIDTH  = width;
    WINDOW_HEIGHT = height;

    glViewport(0, 0, width, height);

    // Update both cameras
    Camera::OnResize(width, height);
    TextRenderer::OnResize(width, height);
}

// ------------------------------------------------------------
// Window creation
// ------------------------------------------------------------
static GLFWwindow* CreateWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Hide window until everything is ready
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 1);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    return glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        "ClubEngine",
        nullptr,
        nullptr
    );
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main()
{
    if (!glfwInit())
        return -1;

    GAME_WINDOW = CreateWindow();
    if (!GAME_WINDOW)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(GAME_WINDOW);

    if (!gladLoadGLLoader(
            reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD\n";
        glfwDestroyWindow(GAME_WINDOW);
        glfwTerminate();
        return -1;
    }

    // --------------------------------------------------------
    // OpenGL global state
    // --------------------------------------------------------
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Optional but recommended
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    // --------------------------------------------------------
    // Callbacks
    // --------------------------------------------------------
    glfwSetFramebufferSizeCallback(
        GAME_WINDOW,
        FramebufferSizeCallback
    );

    // --------------------------------------------------------
    // Engine initialization (NO rendering yet)
    // --------------------------------------------------------
    Camera::Init(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    FONT_MAIN = new Font(
        Path::Font("RobotoMono-Regular.ttf"),
        48
    );

    World::worldTime = glfwGetTime();
    Game::Init();

    // --------------------------------------------------------
    // Show window ONLY when fully ready
    // --------------------------------------------------------
    glfwShowWindow(GAME_WINDOW);

    // --------------------------------------------------------
    // Main loop
    // --------------------------------------------------------
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(GAME_WINDOW))
    {

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        World::Update();
        World::Draw();

        glfwSwapBuffers(GAME_WINDOW);
    }

    // --------------------------------------------------------
    // Shutdown
    // --------------------------------------------------------
    delete FONT_MAIN;
    FONT_MAIN = nullptr;

    glfwDestroyWindow(GAME_WINDOW);
    glfwTerminate();

    return 0;
}
