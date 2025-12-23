#pragma once

#include "Shader.h"
#include "Texture.h"
#include <vector>
#include "FuncLib.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Actor.h"

class Object : public Actor
{
    Shader shader;
    Texture texture;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    VAO vao;
    VBO vbo;
    EBO ebo;

    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    bool matValuesChanged = true;

    public:
    Object(const char* modelPath, const char* texturePath, const char* fragShaderPath);

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    void RefreshModelMatrix();

    Shader* GetShader() override;
    void Draw() override;
};