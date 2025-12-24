#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camera
{
    glm::vec3 Position{};
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 camMat;

    int width, height;

    // Extras for now
    static constexpr float normalSpeed = 0.01f, highSpeed = 0.05f;
    float speed = 0.01f, sensitivity = 100.0f;
    bool firstClick = true;

    Camera(int width, int height, glm::vec3 position);

    public:

    void UpdateMatrix(float FOVdeg, float newPlane, float farPlane);
    void ApplyMatrix(Shader& shader, const char* uniform) const;
    void ProccessInputs(GLFWwindow* window);

    static void Init(int width, int height, glm::vec3 position);
    static void SetPosition(glm::vec3 position);
    static void SetRotation(glm::vec3 rotation);

    static Camera* camera;
};