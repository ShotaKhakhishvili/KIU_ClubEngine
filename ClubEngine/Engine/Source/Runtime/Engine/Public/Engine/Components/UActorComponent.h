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
    UActorComponent() = default;

    virtual void Tick(float dt);
    virtual void OnDestroyed();

    void RegisterComponent();
    void UnregisterComponent();
    bool IsRegistered() const;

    AActor* GetOwner() const;

    bool IsPendingDestroy() const;

protected:
    virtual void OnRegister();
    virtual void OnUnregister();

private:
    friend class ULevel;
    friend class UWorld;

    AActor* owner = nullptr;
    bool registered = false;
};

}