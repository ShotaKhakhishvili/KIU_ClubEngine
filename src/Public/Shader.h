#pragma once

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
    Shader(){}

    ~Shader();

    void Activate() const;
    void Delete();
    void compileErrors(unsigned int shader, std::string type);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    GLuint getID() const;
};