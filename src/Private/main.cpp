#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
#include "VAO.h"
#include "EBO.h"

int width = 800, height = 600;

GLfloat vertices[] = { // Put Square Vertices
    //----Coords-----//    //----Colors-----//
    -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f
};

GLuint indices[] = { // Put Square Indices
    0, 1, 2,
    0, 2, 3
};

GLFWwindow* CreateWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 1);

    // Actually create the window and return it
    return glfwCreateWindow(width, height, "OpenGL_Proj1 Window", nullptr, nullptr);
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

    Shader shader("Shaders/default.vert", "Shaders/default.frag");
    // --------------------------------------------------------------------------------------

    // Create Objects -----------------------------------------------------------------------

    VAO vao;
    vao.Bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    // --------------------------------------------------------------------------------------

    // Uniform ID
    GLuint id_of_offset = glGetUniformLocation(shader.getID(), "offset");
    float offsetY = 0.0f;
    // --------------------------------------------------------------------------------------
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw Triangle ------------------------------------
        shader.Activate();
        glUniform3f(id_of_offset, 0.0f, offsetY, 0.0f);
        vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // --------------------------------------------------
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
             offsetY += 0.01f;
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            offsetY -= 0.01f;
        }

        glfwSwapBuffers(window);
    }

    shader.Delete();

    vao.Delete();
    ebo.Delete();
    vbo.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
}