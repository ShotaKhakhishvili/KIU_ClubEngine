#include "InstancedObject.h"
#include "Path.h"

InstancedObject::InstancedObject(const char* modelPath, const char* texturePath, const char* fragShaderPath) {
    // Shader
    std::string vp = Path::Shader("default.vert");
    std::string fp = Path::Shader(fragShaderPath);
    shader = new Shader(vp.c_str(), fp.c_str());

    ReadFromObjIntoVectors(Path::Model(modelPath), vertices, indices);

    vao = new VAO();
    vao->Bind();

    vbo = new VBO(vertices);
    ebo = new EBO(indices);

    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, coord));
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, color));
    vao->LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));

    VAO::Unbind();
    VBO::Unbind();
    EBO::Unbind();

    texture = new Texture(texturePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture->TexUnit(*shader, "tex0", 0);
}

void InstancedObject::SetPosition(unsigned int id, glm::vec3 p)
{
    auto it = objInsts.find(id);
    if (it == objInsts.end()) return;
    it->second.position = p;
    it->second.matValuesChanged = true;
}

void InstancedObject::SetRotation(unsigned int id, glm::vec3 r) {
    auto it = objInsts.find(id);
    if (it == objInsts.end()) return;
    it->second.rotation = r;
    it->second.matValuesChanged = true;
}
void InstancedObject::SetScale(unsigned int id, glm::vec3 s) {
    auto it = objInsts.find(id);
    if (it == objInsts.end()) return;
    it->second.scale = s;
    it->second.matValuesChanged = true;
}

glm::vec3 InstancedObject::GetPosition(unsigned int id)
{
    auto it = objInsts.find(id);
    if (it == objInsts.end()) return glm::vec3(0.0f);
    return it->second.position;
}

glm::vec3 InstancedObject::GetRotation(unsigned int id)
{
    auto it = objInsts.find(id);
    if (it == objInsts.end()) return glm::vec3(0.0f);
    return it->second.rotation;
}
glm::vec3 InstancedObject::GetScale(unsigned int id)
{
    auto it = objInsts.find(id);
    if (it == objInsts.end()) return glm::vec3(0.0f);
    return it->second.scale;
}

Shader* InstancedObject::GetShader() {
    return shader;
}
void InstancedObject::Draw()
{
    shader->Activate();

    Camera::camera->UpdateMatrix(45.0f, 0.1f, 100.0f);
    Camera::camera->ApplyMatrix(*shader, "camMat");

    texture->Bind();
    vao->Bind();

    GLint modelMatrixLoc = glGetUniformLocation(shader->getID(), "model");

    for (auto& [id, inst] : objInsts)
    {
        if (inst.matValuesChanged) {
            RefreshModelMatrix(inst);
        }

        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(inst.model));
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    }
}


void InstancedObject::RefreshModelMatrix(ObjInst& obj_inst)
{
    shader->Activate();

    glm::mat4 model = glm::mat4(1.0f);

    // Translation
    model = glm::translate(model, obj_inst.position);

    // Rotation (Euler XYZ, degrees)
    model = glm::rotate(model, glm::radians(obj_inst.rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(obj_inst.rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(obj_inst.rotation.z), glm::vec3(0, 0, 1));

    // Scale
    model = glm::scale(model, obj_inst.scale);

    obj_inst.model = model;

    obj_inst.matValuesChanged = false;
}

unsigned int InstancedObject::AddInstance(glm::vec3 position)
{
    unsigned int id = currIndex++;
    objInsts.emplace(id, ObjInst{position});
    return id;
}


InstancedObject::~InstancedObject()
{
    delete vao;
    delete ebo;
    delete vbo;
    delete shader;
    delete texture;
}

void InstancedObject::RemoveInstance(unsigned int index)
{
    auto it = objInsts.find(index);
    if (it == objInsts.end())
        return;

    objInsts.erase(it);
}


void InstancedObject::ClearInstances()
{
    objInsts.clear();
}
