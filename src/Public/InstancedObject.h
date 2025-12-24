#pragma once

#include "Shader.h"
#include "Texture.h"
#include <vector>
#include "FuncLib.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Actor.h"

struct ObjInst {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
    glm::mat4 model{1.0f};

    bool matValuesChanged = true;
};

class InstancedObject : public Actor {

    Shader* shader;
    Texture* texture;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    VAO* vao;
    VBO* vbo;
    EBO* ebo;

    std::vector<ObjInst> objInsts;

    public:

    InstancedObject(const char* modelPath, const char* texturePath, const char* fragShaderPath);
    ~InstancedObject();

    void SetPosition(unsigned int objIndex, glm::vec3 position);
    void SetRotation(unsigned int objIndex, glm::vec3 rotation);
    void SetScale(unsigned int objIndex, glm::vec3 scale);

    glm::vec3 GetPosition(unsigned int objIndex){return objInsts[objIndex].position;}
    glm::vec3 GetRotation(unsigned int objIndex){return objInsts[objIndex].rotation;}
    glm::vec3 GetScale(unsigned int objIndex){return objInsts[objIndex].scale;}

    void RefreshModelMatrix(ObjInst& instRef);

    void Draw() override;
    Shader* GetShader() override;

    void AddInstance(glm::vec3 position);
};