#pragma once

#include "Shader.h"
#include "Texture.h"
#include <vector>
#include "FuncLib.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Actor.h"

struct ModelData
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    VAO* vao;
    VBO* vbo;
    EBO* ebo;
};

class AnimObject : public Actor
{
    Shader* shader;
    Texture* texture;

    std::vector<ModelData> models;
    unsigned int frame = 0;
    float timePassed = 0;
    float frameChangeTime = 0.05;

    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    bool matValuesChanged = true;

public:
    AnimObject(std::vector<const char*> modelPath, const char* texturePath, const char* fragShaderPath);
    ~AnimObject();

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    glm::vec3 GetPosition(){return position;}
    glm::vec3 GetRotation(){return rotation;}
    glm::vec3 GetScale(){return scale;}

    void RefreshModelMatrix();

    Shader* GetShader() override;
    void Draw() override;
    void Update(double dTime) override;
};