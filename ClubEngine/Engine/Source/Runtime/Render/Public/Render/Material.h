#pragma once

#include <Render/Shader.h>
#include <Render/Texture.h>
#include <Render/RenderTypes.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <map>
#include <string>

class Render;

class Material
{
public:
    Material(const std::string&materialName, Shader* shader);
    ~Material();

    Material& operator=(const Material& other) = delete;
    Material(const Material& other) = delete;

    Material& operator=(Material&& other) noexcept;
    Material(Material&& other) noexcept;

    void SetBool    (const std::string& name, const bool    value);
    void SetInt     (const std::string& name, const int32_t value);
    void SetFloat   (const std::string& name, const float   value);
    void SetVec2    (const std::string& name, const float   x,      const float y);
    void SetVec3    (const std::string& name, const float   x,      const float y, const float z);
    void SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float a);
    void SetTexture (const std::string& name, Texture*      texture);

    void Reset();

private:
    static std::map<std::string, int> globalMaterialNames;

    friend class Render;

    void Bind()const;

    std::string runtimeName;

    Shader* shader = nullptr;

    std::map<std::string, bool> bools;
    std::map<std::string, int32_t> ints;
    std::map<std::string, float> floats;
    std::map<std::string, glm::vec2> vectors_2;
    std::map<std::string, glm::vec3> vectors_3;
    std::map<std::string, glm::vec4> vectors_4;
    std::map<std::string, Texture*> textures;

    BlendMode blendMode = BlendMode::Opaque;
    CullMode cullMode = CullMode::Back;
    bool depthWrite = true;
    bool depthTest = true;
};