#include <Core/ClubCore.h>

#include <RHI.OpenGL/GLShader.h>

#include <glad/glad.h>

#include <string>

namespace CE::RHI
{

namespace ShaderUtils
{
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Program
    };

    const char* ToString(ShaderType type)
    {
        switch (type)
        {
            case ShaderType::Vertex:   return "VERTEX";
            case ShaderType::Fragment: return "FRAGMENT";
            case ShaderType::Program:  return "PROGRAM";
            default:                   return "UNKNOWN";
        }
    }

    bool CheckCompileErrors(GLuint shader, ShaderType type)
    {
        GLint success = GL_FALSE;
        char infoLog[512];

        if (type != ShaderType::Program)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                CE_LOG(Error, "[Shader][Type : {}] SHADER ERROR:\n{}", ToString(type), infoLog);
                return false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (success == GL_FALSE)
            {
                glGetProgramInfoLog(shader, 512, nullptr, infoLog);
                CE_LOG(Error, "[Shader][Type : {}] PROGRAM LINK ERROR:\n{}", ToString(ShaderType::Program), infoLog);
                return false;
            }
        }

        return true;
    }

    GLuint CompileShader(const char* source, GLenum glType, ShaderType debugType)
    {
        GLuint shader = glCreateShader(glType);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        if (!CheckCompileErrors(shader, debugType))
        {
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
    {
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        if (!CheckCompileErrors(programID, ShaderType::Program))
        {
            glDeleteProgram(programID);
            return 0;
        }

        return programID;
    }
}

GLShader::GLShader(const char* vertexPath, const char* fragmentPath)
{
    const std::string vertexCode   = CE::FileIO::ReadTextFile(vertexPath);
    const std::string fragmentCode = CE::FileIO::ReadTextFile(fragmentPath);

    const GLuint vertexShader =
        ShaderUtils::CompileShader(vertexCode.c_str(), GL_VERTEX_SHADER, ShaderUtils::ShaderType::Vertex);

    const GLuint fragmentShader =
        ShaderUtils::CompileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER, ShaderUtils::ShaderType::Fragment);

    ID = ShaderUtils::LinkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void GLShader::Bind() const
{
    glUseProgram(ID);
}

void GLShader::Delete()
{
    if (ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0;
    }
}

ShaderID GLShader::GetID() const noexcept
{
    return ID;
}

GLShader::~GLShader()
{
    Delete();
}

GLShader::GLShader(GLShader&& other) noexcept
    : ID(other.ID)
{
    this->uniformLocations = std::move(other.uniformLocations);

    other.uniformLocations.clear();

    other.ID = 0;
}

GLShader& GLShader::operator=(GLShader&& other) noexcept
{
    if (this != &other)
    {
        Delete();

        this->uniformLocations = std::move(other.uniformLocations);

        other.uniformLocations.clear();

        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

int32_t GLShader::GetUniformLocation(const std::string& name)
{
    auto it = uniformLocations.find(name);
    if(it != uniformLocations.end())
        return it->second;

    const int32_t loc = glGetUniformLocation(ID, name.c_str());
    if(loc == -1){
        CE_LOG(Warning, "Uniform \"{}\" not found in GLShader", name); 
    }

    uniformLocations[name] = loc;

    return loc;
}

void GLShader::SetFloat(const std::string& name, const float value)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform1f(location, value);
}

void GLShader::SetInt(const std::string& name, const int32_t value)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform1i(location, value);
}

void GLShader::SetBool(const std::string& name, const bool value)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform1i(location, value ? 1 : 0);
}

void GLShader::SetVec2(const std::string& name, const float x, const float y)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform2f(location, x,y);
}

void GLShader::SetVec3(const std::string& name, const float x, const float y, const float z)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform3f(location, x,y,z);
}

void GLShader::SetVec4(const std::string& name, const float x, const float y, const float z, const float w)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform4f(location, x,y,z,w);
}

void GLShader::SetTexture(const std::string& name, uint32_t slot)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform1i(location, slot);
}

}