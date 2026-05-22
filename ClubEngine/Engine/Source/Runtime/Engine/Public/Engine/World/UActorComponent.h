#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>

namespace CE
{

class AActor;

class UActorComponent : public UObject
{
    GENERATED_BODY(UActorComponent, UObject)

public:
    virtual void Update(float dt);

    void SetOwner(AActor* owner);

private:
    AActor* owner;
};

}