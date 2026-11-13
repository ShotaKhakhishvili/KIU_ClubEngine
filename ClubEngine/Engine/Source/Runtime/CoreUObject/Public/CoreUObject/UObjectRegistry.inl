#pragma once

namespace CE
{

template<typename T, typename... Args>
TObjectHandle<T> UObjectRegistry::Create(Args&&... args)
{
    static_assert(std::is_base_of_v<UObject, T>, "T must derive from UObject");

    return TObjectHandle<T>(Emplace(std::make_unique<T>(std::forward<Args>(args)...)));
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
    const size_t count = slots.size();
    for(size_t i = 0; i < count; i++)
    {
        if(slots[i].alive)
            fn(*(slots[i].object));
    }
}

}