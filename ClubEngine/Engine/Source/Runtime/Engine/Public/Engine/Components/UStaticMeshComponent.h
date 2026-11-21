#pragma once

#include <Engine/Components/UPrimitiveComponent.h>

namespace CE
{

class UStaticMeshComponent : public UPrimitiveComponent
{
    GENERATED_BODY(UStaticMeshComponent, UPrimitiveComponent)

public:
    UStaticMeshComponent() = default;

    void SetStaticMesh(TObjectHandle<UStaticMesh> newStaticMesh);
    TObjectHandle<UStaticMesh> GetStaticMesh() const;

    void SetMaterial(uint32 slot, TObjectHandle<UMaterial> material);
    TObjectHandle<UMaterial> GetMaterial(uint32 slot) const;
    uint32 GetNumMaterials() const;

    void ClearOverrideMaterials();

protected:
    void OnRegister() override;

private:
    void PushOverrideMaterials() const;

    TObjectHandle<UStaticMesh> staticMesh;
    TArray<TObjectHandle<UMaterial>> overrideMaterials;
};

}