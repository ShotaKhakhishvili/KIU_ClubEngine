#pragma once

namespace CE
{

// ============ TPoolHandle ============

template<typename T>
bool TPoolHandle<T>::IsValid() const noexcept
{
    return index != std::numeric_limits<uint32>::max();
}

template<typename T>
void TPoolHandle<T>::Invalidate() noexcept
{
    index = std::numeric_limits<uint32>::max();
    generation = 0;
}

template<typename T>
bool TPoolHandle<T>::operator==(const TPoolHandle& other) const noexcept
{
    return index == other.index && generation == other.generation;
}

// ============ TPool ============

template<typename T>
TPool<T>::TPool(uint32 initialCapacity)
{
    Reserve(initialCapacity);
}

template<typename T>
void TPool<T>::Reserve(uint32 capacity)
{
    items.reserve(capacity);
    itemToSlot.reserve(capacity);
    slots.reserve(capacity);
}

template<typename T>
template<typename... Args>
TPoolHandle<T> TPool<T>::Emplace(Args&&... args)
{
    uint32 slotIndex;

    if(!freeList.empty())
    {
        slotIndex = freeList.back();
        freeList.pop_back();
    }
    else
    {
        slotIndex = static_cast<uint32>(slots.size());
        slots.emplace_back();
    }

    FSlot& slot = slots[slotIndex];
    slot.itemIndex = static_cast<uint32>(items.size());
    slot.alive = true;

    items.emplace_back(std::forward<Args>(args)...);
    itemToSlot.push_back(slotIndex);

    TPoolHandle<T> handle;
    handle.index = slotIndex;
    handle.generation = slot.generation;

    return handle;
}

template<typename T>
TPoolHandle<T> TPool<T>::Add(const T& item)
{
    return Emplace(item);
}

template<typename T>
TPoolHandle<T> TPool<T>::Add(T&& item)
{
    return Emplace(std::move(item));
}

template<typename T>
bool TPool<T>::Remove(TPoolHandle<T> handle)
{
    FSlot* slot = GetSlot(handle);

    if(slot == nullptr)
        return false;

    const uint32 removed = slot->itemIndex;
    const uint32 last    = static_cast<uint32>(items.size() - 1);

    if(removed != last)
    {
        items[removed]      = std::move(items[last]);
        itemToSlot[removed] = itemToSlot[last];

        slots[itemToSlot[removed]].itemIndex = removed;
    }

    items.pop_back();
    itemToSlot.pop_back();

    slot->alive = false;
    ++slot->generation;

    freeList.push_back(handle.index);

    return true;
}

template<typename T>
void TPool<T>::Clear()
{
    for(uint32 i = 0; i < static_cast<uint32>(slots.size()); ++i)
    {
        if(slots[i].alive)
        {
            slots[i].alive = false;
            ++slots[i].generation;
            freeList.push_back(i);
        }
    }

    items.clear();
    itemToSlot.clear();
}

template<typename T>
T* TPool<T>::Resolve(TPoolHandle<T> handle)
{
    FSlot* slot = GetSlot(handle);
    return slot ? &items[slot->itemIndex] : nullptr;
}

template<typename T>
const T* TPool<T>::Resolve(TPoolHandle<T> handle) const
{
    const FSlot* slot = GetSlot(handle);
    return slot ? &items[slot->itemIndex] : nullptr;
}

template<typename T>
bool TPool<T>::Contains(TPoolHandle<T> handle) const
{
    return GetSlot(handle) != nullptr;
}

template<typename T>
uint32 TPool<T>::Num() const noexcept
{
    return static_cast<uint32>(items.size());
}

template<typename T>
bool TPool<T>::IsEmpty() const noexcept
{
    return items.empty();
}

template<typename T>
template<typename F>
void TPool<T>::ForEach(F&& func)
{
    for(T& item : items)
        func(item);
}

template<typename T>
template<typename F>
void TPool<T>::ForEach(F&& func) const
{
    for(const T& item : items)
        func(item);
}

template<typename T>
typename TPool<T>::FSlot* TPool<T>::GetSlot(TPoolHandle<T> handle)
{
    return const_cast<FSlot*>(std::as_const(*this).GetSlot(handle));
}

template<typename T>
const typename TPool<T>::FSlot* TPool<T>::GetSlot(TPoolHandle<T> handle) const
{
    if(!handle.IsValid())
        return nullptr;

    if(handle.index >= static_cast<uint32>(slots.size()))
    {
        CE_LOG(Error, "Tried To Access Pool Item With Handle Out Of Slots Range, Slots Size: {}, Handle Index: {}.",
                    slots.size(), handle.index);
        return nullptr;
    }

    const FSlot& slot = slots[handle.index];

    if(!slot.alive || slot.generation != handle.generation)
        return nullptr;

    return &slot;
}

}
