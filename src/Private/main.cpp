#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtx\matrix_transform_2d.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Path.h"
#include "Shader.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
#include "FuncLib.h"
#include "Object.h"

int width = 800, height = 600;

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


    // -------------------------------General Settings----------------------------------------

    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // -----------------------------------Camera--------------------------------------------------

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 1.0f));

    // -----------------------------------Spawning Objects-------------------------------------

    glm::vec4 lightCol = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 1.0f);

    Object lightSource = Object("Cube.txt", "1.png", "light.frag");
    lightSource.SetPosition(lightPos);

    // Update light's color
    GLint lightColPos = glGetUniformLocation(lightSource.GetShader().getID(), "lightCol");
    glUniform4f(lightColPos, lightCol.x, lightCol.y, lightCol.z, lightCol.w);

    std::vector<Object> objects;

    for (int i = 0; i < 100; i++)
    {
        objects.push_back(Object("Cube.txt", "1.png", "default.frag"));
        objects[i].SetPosition(glm::vec3(i, 0, 0));
        objects[i].SetLightSource(lightPos, lightCol);
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.ProccessInputs(window);

        for (int i = 0; i < objects.size(); i++)
            objects[i].Draw(camera, window);
        lightSource.Draw(camera, window);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}