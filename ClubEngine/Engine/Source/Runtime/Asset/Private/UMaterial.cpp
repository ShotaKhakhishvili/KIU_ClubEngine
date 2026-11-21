#include <Asset/UMaterial.h>
#include <CoreUObject/AssetRegistry.h>

#include <Core/ClubCore.h>

namespace CE
{

namespace
{

TObjectHandle<UMaterial> GDefaultMaterial;

struct FStandardTexture
{
    const char* textureName;
    const char* flagName;
};

constexpr FStandardTexture StandardTextures[] = {
    {"u_BaseColorTexture",         "u_HasBaseColorTexture"},
    {"u_MetallicRoughnessTexture", "u_HasMetallicRoughnessTexture"},
    {"u_NormalTexture",            "u_HasNormalTexture"},
    {"u_OcclusionTexture",         "u_HasOcclusionTexture"},
    {"u_EmissiveTexture",          "u_HasEmissiveTexture"},
};

}

IMPLEMENT_CLASS(UMaterial)

void UMaterial::SetDefault(const TObjectHandle<UMaterial> material)
{
    GDefaultMaterial = material;
}

UMaterial::~UMaterial()
{
    this->Reset();
}

void UMaterial::SetShader(const TObjectHandle<UShader> newShader)
{
    shader = newShader;
}

TObjectHandle<UShader> UMaterial::GetShader()               const { return shader;                  }
const RHI::FMaterialSettings& UMaterial::GetSettings()      const { return settings;                }
const RHI::FRenderStateDesc& UMaterial::GetRenderState()    const { return settings.renderState;    }
TObjectHandle<UMaterial> UMaterial::GetDefault()                  { return GDefaultMaterial;        }

void UMaterial::SetSettings(const RHI::FMaterialSettings& newSettings)
{
    settings = newSettings;
}

bool UMaterial::Bind() const
{
    if(!this->shader.IsValid())
        return false;

    UShader* shader = Resolve(this->shader);

    if(shader == nullptr || !shader->IsReady())
        return false;

    shader->Bind();

    const bool masked = settings.renderState.blendMode == RHI::BlendMode::Masked;

    shader->SetVec4("u_BaseColor", settings.baseColor.x, settings.baseColor.y, settings.baseColor.z, settings.baseColor.w);
    shader->SetFloat("u_Metallic", settings.metallic);
    shader->SetFloat("u_Roughness", settings.roughness);
    shader->SetVec3("u_Emissive", settings.emissive.x, settings.emissive.y, settings.emissive.z);
    shader->SetFloat("u_NormalScale", settings.normalScale);
    shader->SetFloat("u_OcclusionStrength", settings.occlusionStrength);
    shader->SetFloat("u_AlphaCutoff", masked ? settings.alphaCutoff : 0.0f);
    shader->SetBool("u_Unlit", settings.unlit);

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

    for (const auto& [textureName, flagName] : StandardTextures)
    {
        const auto it = textures.find(textureName);

        bool hasTexture = false;

        if (it != textures.end() && it->second.IsValid())
        {
            const UTexture* texture = CE::Resolve(it->second);
            hasTexture = texture != nullptr && texture->IsReady();
        }

        shader->SetBool(flagName, hasTexture);
    }

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

    settings = {};

    shader = TObjectHandle<UShader>{};
}

void UMaterial::SetRenderState(const RHI::FRenderStateDesc& newRenderState)
{
    settings.renderState = newRenderState;
}

void UMaterial::SetBool    (const std::string& name, const bool                 value)    {   bools[name] =     value;  }
void UMaterial::SetInt     (const std::string& name, const int32_t              value)    {   ints[name] =      value;  }
void UMaterial::SetFloat   (const std::string& name, const float                value)    {   floats[name] =    value;  }
void UMaterial::SetTexture (const std::string& name, TObjectHandle<UTexture>    value)    {   textures[name] =  value;  }
void UMaterial::SetVec2    (const std::string& name, const float   x,      const float y)                                { vectors_2[name] = {x,y};      }
void UMaterial::SetVec3    (const std::string& name, const float   x,      const float y, const float z)                 { vectors_3[name] = {x,y,z};    }
void UMaterial::SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float w)  { vectors_4[name] = {x,y,z,w};  }

}