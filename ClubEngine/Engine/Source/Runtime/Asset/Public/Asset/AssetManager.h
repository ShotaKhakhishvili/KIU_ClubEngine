#pragma once

#include <Core/ClubCore.h>
#include <Asset/Handle.h>
#include <Asset/Slot.h>
#include <vector>

template<typename T>
class AssetManager
{
public:
    template<typename ...Args>
    Handle<T> Create(Args&& ...args)
    {
        uint32_t index;

        if(!freeList.empty())
        {
            index = freeList.back();
            freeList.pop_back();
        }
        else
        {
            index = slots.size();
            slots.emplace_back();
        }

        auto& slot = slots[index];
        slot.asset = std::make_unique(std::forward<Args>(args)...);
        slot.alive = true;

        return {index, slot.generation};
    };

    T* Resolve(const Handle<T>& handle) const
    {
        if(handle.index >= slots.size())
        {
            CE_LOG(Warning, "Tried To Access Invalid Asset Index");
            return nullptr;
        }

        auto& slot = slots[handle.index];

        if(!slot.alive)
        {
            CE_LOG(Warning, "Tried To Access Unalived Asset");
            return nullptr;
        }

        if(slot.generation != handle.generation)
        {
            CE_LOG(Warning, "Tried To Access Unloaded Asset");
            return nullptr;
        }

        return slot.asset.get();
    }


private:
    std::vector<Slot<T>> slots;
    std::vector<uint32_t> freeList;
};