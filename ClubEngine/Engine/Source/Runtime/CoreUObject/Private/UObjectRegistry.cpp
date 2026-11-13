#include <CoreUObject/UObjectRegistry.h>
#include <Core/ClubCore.h>

namespace CE
{

uint32 UObjectRegistry::registryCounter = 0;

void UObjectRegistry::Destroy(const UObjectHandle handle)
{
    if (!IsAlive(handle))
        return;

    Slot* slot = GetSlotFromHandle(handle);
    
    if(slot == nullptr) 
        return;

    slot->alive = false;
    ++slot->generation;

    pendingDestroy.push_back(handle.index);
}

UObjectHandle UObjectRegistry::Create(const UClass* objectClass)
{
    if (!objectClass)
        return {};

    std::unique_ptr<UObject> object = objectClass->Construct();

    if (!object)
    {
        CE_LOG(Error, "Tried To Construct Abstract Class {}.", objectClass->GetName());
        return {};
    }

    return Emplace(std::move(object));
}

UObjectHandle UObjectRegistry::Emplace(std::unique_ptr<UObject> object)
{
    uint32_t index;

    if(!freeList.empty())
    {
        index = freeList.back();
        freeList.pop_back();
    }
    else
    {
        index = static_cast<uint32_t>(slots.size());
        slots.emplace_back();
    }

    Slot& slot = slots[index];
    slot.object = std::move(object);
    slot.alive = true;

    slot.object->id = ++nextID;
    slot.object->SetName(nullptr);

    UObjectHandle handle{};

    handle.index = index;
    handle.generation = slot.generation;
    handle.registryId = registryId;

    slot.object->selfHandle = handle;

    return handle;
}

UObjectRegistry::UObjectRegistry()
{
    this->registryId = registryCounter++;
}

UObject* UObjectRegistry::Resolve(const UObjectHandle handle)
{
    if (handle.registryId != registryId)
        return nullptr;

    return ResolveRaw(handle);
}

UObject* UObjectRegistry::ResolveRaw(const UObjectHandle handle)
{
    const Slot* slot = GetSlotFromHandle(handle);

    if(slot == nullptr)
        return nullptr;

    return slot->object.get();
}

UObjectRegistry::Slot* UObjectRegistry::GetSlotFromHandle(const UObjectHandle handle)
{
    if(static_cast<uint32_t>(slots.size()) <= handle.index)
    {
        CE_LOG(Error, "Tried To Access Object With Handle Out Of Slots Range, Slots Size: {}, Handle Index: {}.", 
                    slots.size(), handle.index);
        return nullptr;
    }

    Slot& slot = slots[handle.index];

    if(slot.generation != handle.generation)
    {
        CE_LOG(Warning, "Tried To Access Object With Mismatching Slot Generation, Slot Generation: {}, Handle Generation: {}.",
                    slot.generation, handle.generation);
        return nullptr;
    }

    if(!slot.alive)
    {
        CE_LOG(Warning, "Tried To Access Destroyed Object. Operation Will Be Ignored.");
        return nullptr;
    }

    return &slot;
}

uint32 UObjectRegistry::GetRegistryId() const
{
    return registryId;
}

bool UObjectRegistry::IsAlive(const UObjectHandle handle) const
{
    if (handle.registryId != registryId)
        return false;

    if (handle.index >= static_cast<uint32>(slots.size()))
        return false;

    const Slot& slot = slots[handle.index];
    return slot.alive && slot.generation == handle.generation;
}

void UObjectRegistry::FlushPendingDestroy()
{
    for(const uint32 index : pendingDestroy)
    {
        slots[index].object.reset();
        freeList.push_back(index);
    }

    pendingDestroy.clear();
}

}