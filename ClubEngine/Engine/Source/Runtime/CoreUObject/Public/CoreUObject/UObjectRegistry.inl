#pragma once

namespace CE
{

template<typename T, typename... Args>
TObjectHandle<T> UObjectRegistry::Create(Args&&... args)
{
    static_assert(std::is_base_of_v<UObject, T>, "T must derive from UObject");

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
    slot.object = std::make_unique<T>(std::forward<Args>(args)...);
    slot.alive = true;

    slot.object->id = ++nextID;

    UObjectHandle handle{};

    handle.index = index;
    handle.generation = slot.generation;
    handle.registryId = registryId;

    slot.object->selfHandle = handle;

    return TObjectHandle<T>(handle);
}

template<typename T>
T* UObjectRegistry::Resolve(TObjectHandle<T> handle)
{
    if (handle.GetRegistryID() != registryId)
        return nullptr;

    return static_cast<T*>(ResolveRaw(handle.handle));
}

template<typename F>
void UObjectRegistry::ForEach(F&& fn)
{
    ++iterationDepth;

    size_t count = slots.size();
    for(size_t i = 0; i < count; i++)
    {
        if(slots[i].alive)
            fn(*(slots[i].object));
    }

    if(--iterationDepth == 0)
        FlushPendingDestroy();
}

}