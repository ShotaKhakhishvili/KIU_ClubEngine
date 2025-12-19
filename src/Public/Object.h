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

    public:
    Object(const char* modelPath, const char* texturePath);

    void Draw(Camera& camera, GLFWwindow* window);
};