#include <Render/Material.h>
#include <Core/ClubCore.h>

std::unordered_map<std::string, int> Material::globalMaterialNames{};

Material::Material(const std::string& materialName, Handle<Shader> shader)
{
    const int nameId = globalMaterialNames[materialName]++;
    this->runtimeName = materialName + std::to_string(nameId);

    this->shader = shader;
}

Material::Material(Material&& other) noexcept
{
    this->runtimeName = std::move(other.runtimeName);

    this->shader = other.shader;

    this->bools      = std::move(other.bools);
    this->ints       = std::move(other.ints);
    this->floats     = std::move(other.floats);
    this->vectors_2  = std::move(other.vectors_2);
    this->vectors_3  = std::move(other.vectors_3);
    this->vectors_4  = std::move(other.vectors_4);
    this->textures   = std::move(other.textures);

    this->blendMode  = other.blendMode;
    this->cullMode   = other.cullMode;
    this->depthWrite = other.depthWrite;
    this->depthTest  = other.depthTest;

    other.Reset();
}

Material::~Material()
{
    this->Reset();
}

void Material::Bind()const
{
    if(!this->shader.IsValid())
        return;

    Shader* shader = GetShaders().Resolve(this->shader);

    if(shader == nullptr)
        return;

    shader->Bind();

    for (const auto& [name, v] : bools)
        shader->SetBool(name, v);

    for (const auto& [name, v] : ints)
        shader->SetInt(name, v);

    for (const auto& [name, v] : floats)
        shader->SetFloat(name, v);

    for (const auto& [name, v] : vectors_2)
        shader->SetVec2(name, v.x, v.y);

    for (const auto& [name, v] : vectors_3)
        shader->SetVec3(name, v.x, v.y, v.z);

    for (const auto& [name, v] : vectors_4)
        shader->SetVec4(name, v.x, v.y, v.z, v.a);

    uint16_t slot = 0;

    for(const auto& [name, tex] : textures)
    {
        if(!tex.IsValid())
            continue;

        Texture* texture = GetTextures().Resolve(tex);

        if(texture == nullptr)
            continue;
        
        texture->Bind(slot);
        shader->SetInt(name, slot);
        ++slot;
    }
}

Material& Material::operator=(Material&& other) noexcept
{
    if(&other == this)
        return *this;

    this->Reset();

    this->runtimeName = std::move(other.runtimeName);

    this->shader = other.shader;

    this->bools      = std::move(other.bools);
    this->ints       = std::move(other.ints);
    this->floats     = std::move(other.floats);
    this->vectors_2  = std::move(other.vectors_2);
    this->vectors_3  = std::move(other.vectors_3);
    this->vectors_4  = std::move(other.vectors_4);
    this->textures   = std::move(other.textures);

    this->blendMode  = other.blendMode;
    this->cullMode   = other.cullMode;
    this->depthWrite = other.depthWrite;
    this->depthTest  = other.depthTest;

    other.Reset();

    return *this;
}

void Material::Reset()
{
    bools.clear();
    ints.clear();
    floats.clear();
    textures.clear();
    vectors_2.clear();
    vectors_3.clear();
    vectors_4.clear();
    
    blendMode = BlendMode::Opaque;
    cullMode = CullMode::Back;
    depthWrite = true;
    depthTest = true;
    shader.Invalidate();
}

void Material::SetBool    (const std::string& name, const bool          value)    {   bools[name] =       value;  }
void Material::SetInt     (const std::string& name, const int32_t       value)    {   ints[name] =        value;  }
void Material::SetFloat   (const std::string& name, const float         value)    {   floats[name] =      value;  }
void Material::SetTexture (const std::string& name, Handle<Texture>     value)    {   textures[name] =    value;  }
void Material::SetVec2    (const std::string& name, const float   x,      const float y)                                { vectors_2[name] = {x,y};      }
void Material::SetVec3    (const std::string& name, const float   x,      const float y, const float z)                 { vectors_3[name] = {x,y,z};    }
void Material::SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float a)  { vectors_4[name] = {x,y,z,a};  }