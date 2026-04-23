#include <Asset/UMaterial.h>
#include <Asset/AssetAccess.h>

#include <Core/ClubCore.h>

UMaterial::UMaterial(TObjectHandle<UShader> inShader)
    : UObject("UMaterial"), shader(inShader)
    {
    
    }

UMaterial::UMaterial(UMaterial&& other) noexcept
    : UObject("UMaterial")
{
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

UMaterial::~UMaterial()
{
    this->Reset();
}

void UMaterial::Bind() const
{
    if(!this->shader.IsValid())
        return;

    UShader* shader = CE::ResolveShader(this->shader);

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
        shader->SetVec4(name, v.x, v.y, v.z, v.w);

    uint16_t slot = 0;

    for(const auto& [name, tex] : textures)
    {
        if(!tex.IsValid())
            continue;

        UTexture* texture = CE::ResolveTexture(tex);

        if(texture == nullptr)
            continue;
        
        texture->Bind(slot);
        shader->SetInt(name, slot);
        ++slot;
    }
}

UMaterial& UMaterial::operator=(UMaterial&& other) noexcept
{
    if(&other == this)
        return *this;

    this->Reset();

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

void UMaterial::Reset()
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
    shader = TObjectHandle<UShader>{};
}

void UMaterial::SetBool    (const std::string& name, const bool                 value)    {   bools[name] =     value;  }
void UMaterial::SetInt     (const std::string& name, const int32_t              value)    {   ints[name] =      value;  }
void UMaterial::SetFloat   (const std::string& name, const float                value)    {   floats[name] =    value;  }
void UMaterial::SetTexture (const std::string& name, TObjectHandle<UTexture>    value)    {   textures[name] =  value;  }
void UMaterial::SetVec2    (const std::string& name, const float   x,      const float y)                                { vectors_2[name] = {x,y};      }
void UMaterial::SetVec3    (const std::string& name, const float   x,      const float y, const float z)                 { vectors_3[name] = {x,y,z};    }
void UMaterial::SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float w)  { vectors_4[name] = {x,y,z,w};  }