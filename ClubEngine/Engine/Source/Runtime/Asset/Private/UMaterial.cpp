#include <Asset/UMaterial.h>
#include <Asset/AssetAccess.h>

#include <Core/ClubCore.h>

namespace CE
{

UMaterial::UMaterial(TObjectHandle<UShader> inShader)
    : shader(inShader)
{
}

UMaterial::~UMaterial()
{
    this->Reset();
}

bool UMaterial::Bind() const
{
    if(!this->shader.IsValid())
        return false;

    UShader* shader = Resolve(this->shader);

    if(shader == nullptr || !shader->IsReady())
        return false;

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

        UTexture* texture = CE::Resolve(tex);

        if(texture == nullptr)
            continue;
        
        texture->Bind(slot);
        shader->SetInt(name, slot);
        ++slot;
    }

    return true;
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
    
    renderState.blendMode = RHI::BlendMode::Opaque;
    renderState.cullMode = RHI::CullMode::Back;
    renderState.depthWrite = true;
    renderState.depthTest = true;

    shader = TObjectHandle<UShader>{};
}
const FMaterialRenderState& UMaterial::GetRenderState() const
{
    return renderState;
}
void UMaterial::SetRenderState(const FMaterialRenderState& newRenderState)
{
    this->renderState = newRenderState;
}

void UMaterial::SetBool    (const std::string& name, const bool                 value)    {   bools[name] =     value;  }
void UMaterial::SetInt     (const std::string& name, const int32_t              value)    {   ints[name] =      value;  }
void UMaterial::SetFloat   (const std::string& name, const float                value)    {   floats[name] =    value;  }
void UMaterial::SetTexture (const std::string& name, TObjectHandle<UTexture>    value)    {   textures[name] =  value;  }
void UMaterial::SetVec2    (const std::string& name, const float   x,      const float y)                                { vectors_2[name] = {x,y};      }
void UMaterial::SetVec3    (const std::string& name, const float   x,      const float y, const float z)                 { vectors_3[name] = {x,y,z};    }
void UMaterial::SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float w)  { vectors_4[name] = {x,y,z,w};  }

}