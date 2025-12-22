#pragma one

#include "Shader.h"
#include "Texture.h"
#include <vector>
#include "FuncLib.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"

class Object
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

    void SetLightSource(glm::vec3 position, glm::vec4 color);

    Shader& GetShader();
    void Draw(Camera& camera);
};