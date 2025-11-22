#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int width = 800, height = 600;

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

GLfloat vertices[] = {
  -0.5f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f
};

GLFWwindow* CreateWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 1);

    // Actually create the window and return it
    return glfwCreateWindow(width, height, "OpenGL_Proj1 Window", nullptr, nullptr);
}

GLuint CreateVertexShader() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    return vertexShader;
}

GLuint CreateFragmentShader() {
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    return fragmentShader;
}

GLuint CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    return shaderProgram;
}

GLuint CreateVAO() {
    GLuint vao;

    glad_glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return vao;
}

GLuint CreateVBO() {
    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_COPY);

    return vbo;
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
    else {
        std::cout << "Successfully created" << std::endl;
    }

    glfwMakeContextCurrent(window); // Actually display the window

    gladLoadGL(); // Load Functions

    // Create Shaders -----------------------------------------------------------------------

    GLuint vertexShader = CreateVertexShader();
    GLuint fragmentShader = CreateFragmentShader();
    GLuint shaderProgram = CreateShaderProgram(vertexShader, fragmentShader);

    // --------------------------------------------------------------------------------------

    // Create Objects -----------------------------------------------------------------------

    GLuint vao = CreateVAO();
    GLuint vbo = CreateVBO();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --------------------------------------------------------------------------------------

    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw Triangle ------------------------------------
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // --------------------------------------------------

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}