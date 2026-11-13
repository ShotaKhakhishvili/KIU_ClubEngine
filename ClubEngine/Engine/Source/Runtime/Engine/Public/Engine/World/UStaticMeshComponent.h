#pragma once

#include <Engine/World/UPrimitiveComponent.h>

namespace CE
{

class UStaticMeshComponent : public UPrimitiveComponent
{
    GENERATED_BODY(UStaticMeshComponent, UPrimitiveComponent)

public:
    UStaticMeshComponent() = default;

    void SetStaticMesh(TObjectHandle<UStaticMesh> newStaticMesh);
    TObjectHandle<UStaticMesh> GetStaticMesh() const;

protected:
    void OnRegister() override;

private:
    TObjectHandle<UStaticMesh> staticMesh;
};

}