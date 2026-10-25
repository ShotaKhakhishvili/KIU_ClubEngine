#pragma once

#include <Core/Types/CoreTypes.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/UObjectHandle.h>
#include <CoreUObject/TObjectHandle.h>

#include <vector>
#include <memory>

namespace CE
{

class UObjectRegistry
{
public:

    UObjectRegistry();

    UObject* Resolve(UObjectHandle handle);
    void Destroy(UObjectHandle handle);

    template<typename T, typename... Args>
    TObjectHandle<T> Create(Args&&... args);

    template<typename T>
    T* Resolve(TObjectHandle<T> handle);

    template<typename F>
    void ForEach(F&& fn);

    uint32 GetRegistryId() const;

private:
    void FlushPendingDestroy();

    struct Slot
    {
        std::unique_ptr<UObject> object;
        uint32 generation = 0;
        bool alive = false;
    };

    UObject* ResolveRaw(UObjectHandle handle);
    Slot* GetSlotFromHandle(UObjectHandle handle);

    size_t iterationDepth = 0;
    std::vector<uint32> pendingDestroy;

    std::vector<uint32> freeList;
    std::vector<Slot> slots;
    uint64 nextID = 0;

    static uint32 registryCounter;
    uint32 registryId;
};

}

#include "UObjectRegistry.inl"