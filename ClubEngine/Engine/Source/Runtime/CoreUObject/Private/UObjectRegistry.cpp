#include <CoreUObject/UObjectRegistry.h>
#include <Core/ClubCore.h>

namespace CE
{

void UObjectRegistry::Destroy(UObjectHandle handle)
{
    Slot* slot = GetSlotFromHandle(handle);
    
    if(slot == nullptr) 
        return;

    slot->alive = false;
    ++slot->generation;

    if(iterationDepth != 0)
    {
        pendingDestroy.push_back(handle.index);
        return;
    }

    slot->object.reset();

    freeList.push_back(handle.index);
}

UObject* UObjectRegistry::Resolve(UObjectHandle handle)
{
    return ResolveRaw(handle);
}

UObject* UObjectRegistry::ResolveRaw(UObjectHandle handle) 
{
    const Slot* slot = GetSlotFromHandle(handle);

    if(slot == nullptr)
        return nullptr;

    return slot->object.get();
}

UObjectRegistry::Slot* UObjectRegistry::GetSlotFromHandle(UObjectHandle handle)
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