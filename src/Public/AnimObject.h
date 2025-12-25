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

    std::vector<std::vector<ModelData>> animations;

    unsigned int animationIndex = 0;
    unsigned int nextAnimationIndex = 0;
    unsigned int frame = 0;
    unsigned int ignoredLastFrames = 0;
    float timePassed = 0;
    float frameChangeTime = 0.05;
    float animSpeed = 1.0f;

    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    bool matValuesChanged = true;

public:
    AnimObject(const std::vector<const char*>& modelsPrefix, const std::vector<unsigned int>& modelCount, const char* texturePath, const char* fragShaderPath);
    ~AnimObject();

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(glm::vec3 scale);

    glm::vec3 GetPosition(){return position;}
    glm::vec3 GetRotation(){return rotation;}
    glm::vec3 GetScale(){return scale;}

    void RefreshModelMatrix();

    void PlayAnimationOnce(unsigned int animationIndex, unsigned int frame, unsigned int ignoredLastFrames, unsigned int nextAnimationIndex, float animSpeed);

    unsigned int GetAnimIndex(){return animationIndex;}

    Shader* GetShader() override;
    void Draw() override;
    void Update(double dTime) override;
};