#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Path.h"
#include "Shader.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "stb/stb_image.h"

int width = 800, height = 600;

GLfloat vertices[] = { // Put Square Vertices
    //----Coords-----//    //----Colors-----//
    -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f,     // UpperLeft
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     // LowerLeft
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // LowerRight
     0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 1.0f      // UpperRight
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

    Shader shader(
        (Path::Shader("default.vert").c_str()),
        (Path::Shader("default.frag").c_str())
    );

    // --------------------------------------------------------------------------------------

    // Create Objects -----------------------------------------------------------------------

    VAO vao;
    vao.Bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    // --------------------------------------------------------------------------------------
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    Texture texture = Texture("1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw Triangle ------------------------------------
        shader.Activate();
        glBindTexture(GL_TEXTURE_2D, texture.getID());
        vao.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    shader.Delete();

    vao.Delete();
    ebo.Delete();
    vbo.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
}