#include "InstancedObject.h"
#include "Path.h"

InstancedObject::InstancedObject(const char* modelPath, const char* texturePath, const char* fragShaderPath) {
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

void InstancedObject::SetPosition(signed int objIndex, glm::vec3 position) {
    objInsts[objIndex].position = position;
    objInsts[objIndex].matValuesChanged = false;
}
void InstancedObject::SetRotation(signed int objIndex, glm::vec3 rotation) {
    objInsts[objIndex].rotation = rotation;
    objInsts[objIndex].matValuesChanged = false;
}
void InstancedObject::SetScale(signed int objIndex, glm::vec3 scale) {
    objInsts[objIndex].scale = scale;
    objInsts[objIndex].matValuesChanged = false;
}

void InstancedObject::SetLightSource(glm::vec3 position, glm::vec4 color) {
    shader.Activate();

    GLint positionLoc = glGetUniformLocation(shader.getID(), "lightPos");
    glUniform3f(positionLoc, position.x, position.y, position.z);
    GLint colorLoc = glGetUniformLocation(shader.getID(), "lightCol");
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
}

Shader& InstancedObject::GetShader() {
    return shader;
}
void InstancedObject::Draw(Camera& camera) {
    shader.Activate();

    camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
    camera.ApplyMatrix(shader, "camMat");

    texture.Bind();
    vao.Bind();

    GLint modelMatrixLoc = glGetUniformLocation(shader.getID(), "model");

    for (int i = 0; i < objInsts.size(); i++) {
        if (objInsts[i].matValuesChanged) {
            RefreshModelMatrix(objInsts[i]);
            objInsts[i].matValuesChanged = false;
        }
        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(objInsts[i].model));
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void InstancedObject::RefreshModelMatrix(ObjInst& obj_inst)
{
    shader.Activate();

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

void InstancedObject::AddInstance(glm::vec3 position){
    objInsts.push_back(ObjInst(position));
}