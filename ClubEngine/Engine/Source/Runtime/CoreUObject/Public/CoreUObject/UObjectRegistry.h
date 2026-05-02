#pragma once

#include <CoreUObject/UObject.h>
#include <CoreUObject/UObjectHandle.h>
#include <CoreUObject/TObjectHandle.h>

#include <vector>
#include <memory>
#include <type_traits>

namespace CE
{

class UObjectRegistry
{
public:

    UObject* Resolve(const UObjectHandle& handle);
    void Destroy(const UObjectHandle& handle);

    template<typename T, typename... Args>
    TObjectHandle<T> Create(Args&&... args)
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

        return TObjectHandle<T>(handle);
    }
    
    template<typename T>
    T* Resolve(const TObjectHandle<T>& handle)
    {
        return static_cast<T*>(ResolveRaw(handle.handle));
    }

private:
    struct Slot
    {
        std::unique_ptr<UObject> object;
        uint32_t generation = 0;
        bool alive = false;
    };

    UObject* ResolveRaw(const UObjectHandle& handle);
    Slot* GetSlotFromHandle(const UObjectHandle& handle);

    std::vector<uint32_t> freeList;
    std::vector<Slot> slots;
    uint64_t nextID = 0;
};

}