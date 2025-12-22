#include "Object.h"
#include "Path.h"

Object::Object(const char* modelPath, const char* texturePath, const char* fragShaderPath)
{
    // Shader
    std::string vp = Path::Shader("default.vert");
    std::string fp = Path::Shader(fragShaderPath);
    shader = Shader(vp.c_str(), fp.c_str());

    ReadFromObjIntoVectors(Path::Model(modelPath), vertices, indices);

    vao.Bind();

    vbo = VBO(vertices);
    ebo = EBO(indices);

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, coord));
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
    vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));

    VAO::Unbind();
    VBO::Unbind();
    EBO::Unbind();

    texture = Texture(texturePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture.TexUnit(shader, "tex0", 0);
}

void Object::Draw(Camera& camera, GLFWwindow* window)
{
    if (matValuesChanged)
        RefreshModelMatrix();
    shader.Activate();

    camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
    camera.ApplyMatrix(shader, "camMat");

    texture.Bind();
    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Object::SetPosition(glm::vec3 newPos)
{
    this->position = newPos;
    matValuesChanged = true;
}
void Object::SetRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
    matValuesChanged = true;
}
void Object::SetScale(glm::vec3 scale)
{
    this->scale = scale;
    matValuesChanged = true;
}
void Object::RefreshModelMatrix()
{
    shader.Activate();

    glm::mat4 model = glm::mat4(1.0f);

    // Translation
    model = glm::translate(model, position);

    // Rotation (Euler XYZ, degrees)
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));

    // Scale
    model = glm::scale(model, scale);

    GLint modelLoc = glGetUniformLocation(shader.getID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Object::SetLightSource(glm::vec3 position, glm::vec4 color)
{
    shader.Activate();

    GLint positionLoc = glGetUniformLocation(shader.getID(), "lightPos");
    glUniform3f(positionLoc, position.x, position.y, position.z);
    GLint colorLoc = glGetUniformLocation(shader.getID(), "lightCol");
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
}

Shader& Object::GetShader()
{
    return shader;
}
