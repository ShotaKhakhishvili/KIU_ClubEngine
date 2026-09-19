#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/TObjectHandle.h>

namespace CE
{

class AActor;

class UActorComponent : public UObject
{
    GENERATED_BODY(UActorComponent, UObject)

public:
    virtual void Tick(float dt);

    void SetOwner(TObjectHandle<AActor> owner);

private:
    TObjectHandle<AActor> owner;
};

}