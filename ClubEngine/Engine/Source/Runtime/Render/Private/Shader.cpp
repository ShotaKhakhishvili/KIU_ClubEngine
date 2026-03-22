#include <Shader.h>
#include <Core/ClubCore.h>

#include <string>

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
                //CE_LOG(Error, "[Shader][Type : {}] SHADER ERROR:\n{}", ToString(type), infoLog);
                return false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (success == GL_FALSE)
            {
                glGetProgramInfoLog(shader, 512, nullptr, infoLog);
                //CE_LOG(Error, "[Shader][Type : {}] PROGRAM LINK ERROR:\n{}", ToString(ShaderType::Program), infoLog);
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

Shader::Shader(const char* vertexPath, const char* fragmentPath)
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

void Shader::Activate() const
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    if (ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0;
    }
}

GLuint Shader::GetID() const noexcept
{
    return ID;
}

Shader::~Shader()
{
    Delete();
}

Shader::Shader(Shader&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        Delete();
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}