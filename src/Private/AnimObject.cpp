#include "AnimObject.h"
#include "Path.h"

AnimObject::AnimObject(std::vector<const char*> modelPaths, const char* texturePath, const char* fragShaderPath)
{
    // Shader
    std::string vp = Path::Shader("default.vert");
    std::string fp = Path::Shader(fragShaderPath);
    shader = new Shader(vp.c_str(), fp.c_str());

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (signed int i = 0; i < modelPaths.size(); ++i)
    {
        ReadFromObjIntoVectors(Path::Model(modelPaths[i]), vertices, indices);

        VAO* vao = new VAO();
        vao->Bind();

        VBO* vbo = new VBO(vertices);
        EBO* ebo = new EBO(indices);

        vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, coord));
        vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
        vao->LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));

        VAO::Unbind();
        VBO::Unbind();
        EBO::Unbind();

        models.push_back(ModelData(vertices, indices, vao, vbo, ebo));
    }

    texture = new Texture(texturePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture->TexUnit(*shader, "tex0", 0);
}

void AnimObject::Draw()
{
    shader->Activate();

    RefreshModelMatrix();

    Camera::camera->UpdateMatrix(45.0f, 0.1f, 100.0f);
    Camera::camera->ApplyMatrix(*shader, "camMat");

    texture->Bind();
    models[frame].vao->Bind();
    glDrawElements(GL_TRIANGLES, models[frame].indices.size(), GL_UNSIGNED_INT, 0);
}

void AnimObject::SetPosition(glm::vec3 newPos)
{
    this->position = newPos;
    matValuesChanged = true;
}
void AnimObject::SetRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
    matValuesChanged = true;
}
void AnimObject::SetScale(glm::vec3 scale)
{
    this->scale = scale;
    matValuesChanged = true;
}
void AnimObject::RefreshModelMatrix()
{
    shader->Activate();

    glm::mat4 model = glm::mat4(1.0f);

    // Translation
    model = glm::translate(model, position);

    // Rotation (Euler XYZ, degrees)
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));

    // Scale
    model = glm::scale(model, scale);

    GLint modelLoc = glGetUniformLocation(shader->getID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    matValuesChanged = false;
}

Shader* AnimObject::GetShader()
{
    return shader;
}

AnimObject::~AnimObject()
{
    delete shader;
    delete texture;
}


void AnimObject::Update(double dTime)
{
    if (models.empty() || frameChangeTime <= 0)
        return;

    timePassed += static_cast<float>(dTime);

    const int deltaFrames = static_cast<int>(timePassed / frameChangeTime);

    if (deltaFrames > 0)
    {
        timePassed -= static_cast<float>(deltaFrames) * frameChangeTime;
        frame = (frame + deltaFrames) % static_cast<int>(models.size());
    }
}
