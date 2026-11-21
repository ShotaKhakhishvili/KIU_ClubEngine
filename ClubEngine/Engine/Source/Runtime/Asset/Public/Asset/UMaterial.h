#pragma once

#include <Core/Math/Vector.h>

#include <CoreUObject/TObjectHandle.h>

#include <Asset/UShader.h>
#include <Asset/UTexture.h>

#include <string>
#include <unordered_map>

namespace CE
{

class UMaterial : public UObject
{
    GENERATED_BODY(UMaterial, UObject)

public:
    
    UMaterial() = default;
    ~UMaterial() override;

    void SetShader(TObjectHandle<UShader> newShader);
    TObjectHandle<UShader> GetShader() const;

    void SetSettings(const RHI::FMaterialSettings& newSettings);
    const RHI::FMaterialSettings& GetSettings() const;

    void SetBool    (const std::string& name, bool    value);
    void SetInt     (const std::string& name, int32_t value);
    void SetFloat   (const std::string& name, float   value);
    void SetVec2    (const std::string& name, float   x,      float y);
    void SetVec3    (const std::string& name, float   x,      float y, float z);
    void SetVec4    (const std::string& name, float   x,      float y, float z, float w);
    void SetTexture (const std::string& name, TObjectHandle<UTexture>      texture);

    bool Bind() const;

    void Reset();

    const RHI::FRenderStateDesc& GetRenderState() const;
    void SetRenderState(const RHI::FRenderStateDesc& newRenderState);

    static void SetDefault(TObjectHandle<UMaterial> material);
    static TObjectHandle<UMaterial> GetDefault();

private:
    TObjectHandle<UShader> shader;
    RHI::FMaterialSettings settings;

    std::unordered_map<std::string, bool> bools;
    std::unordered_map<std::string, int32_t> ints;
    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, FVector2F> vectors_2;
    std::unordered_map<std::string, FVectorF> vectors_3;
    std::unordered_map<std::string, FVector4F> vectors_4;
    std::unordered_map<std::string, TObjectHandle<UTexture>> textures;
};

}