#include <CoreUObject/UObjectRegistry.h>
#include <Core/ClubCore.h>

namespace CE
{

void UObjectRegistry::Destroy(const UObjectHandle& handle)
{
    Slot* slot = GetSlotFromHandle(handle);
    
    if(slot == nullptr) 
        return;

    slot->object.reset();
    slot->alive = false;
    ++slot->generation;

    freeList.push_back(handle.index);
}

UObject* UObjectRegistry::Resolve(const UObjectHandle& handle)
{
    return ResolveRaw(handle);
}

UObject* UObjectRegistry::ResolveRaw(const UObjectHandle& handle) 
{
    const Slot* slot = GetSlotFromHandle(handle);

    if(slot == nullptr)
        return nullptr;

    return slot->object.get();
}

UObjectRegistry::Slot* UObjectRegistry::GetSlotFromHandle(const UObjectHandle& handle)
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

}