#pragma once

#define SHADER_PATH(name) (std::string(PROJECT_DIR) + "/src/Private/Shaders/" + name).c_str()

#include <glad/glad.h>
#include <string>
#include<fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string readFile(std::string fileName);

class Shader
{
    GLuint ID;

    public:
    Shader(const char* vertexPath, const char* fragmentPath);

    void Activate() const;
    void Delete() const;
    void compileErrors(unsigned int shader, std::string type);

    GLuint getID() const;
};