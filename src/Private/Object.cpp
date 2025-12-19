#include "Object.h"
#include "Path.h"

Object::Object(const char* modelPath, const char* texturePath)
{
    // Shader
    std::string vp = Path::Shader("default.vert");
    std::string fp = Path::Shader("default.frag");
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
    shader.Activate();

    camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
    camera.ApplyMatrix(shader, "camMat");

    texture.Bind();
    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}