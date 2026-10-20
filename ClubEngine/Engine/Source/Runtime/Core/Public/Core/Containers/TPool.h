#pragma once

#include <Core/Log.h>
#include <Core/Types/CoreTypes.h>
#include <Core/Containers/TArray.h>

#include <limits>
#include <utility>

namespace CE
{

template<typename T>
class TPool;

template<typename T>
class TPoolHandle
{
public:
    bool IsValid() const noexcept;
    void Invalidate() noexcept;

    bool operator==(const TPoolHandle& other) const noexcept;

private:
    friend class TPool<T>;

    uint32 index = std::numeric_limits<uint32>::max();
    uint32 generation = 0;
};

template<typename T>
class TPool
{
public:
    TPool() = default;

    explicit            TPool           (uint32 initialCapacity);
    void                Reserve         (uint32 capacity);

    template<typename... Args>
    TPoolHandle<T>      Emplace         (   Args&&... args);

    TPoolHandle<T>      Add             (const T& item);
    TPoolHandle<T>      Add             (T&& item);

    bool Remove(TPoolHandle<T> handle);

    void Clear();

    T*                  Resolve         (TPoolHandle<T> handle);
    const T*            Resolve         (TPoolHandle<T> handle) const;
    bool                Contains        (TPoolHandle<T> handle) const;

    uint32              Num             () const noexcept;
    bool                IsEmpty         () const noexcept;

    template<typename F>
    void ForEach(F&& func);

    template<typename F>
    void                ForEach         (F&& func) const;

    auto                begin           ()       noexcept { return items.begin(); }
    auto                end             ()         noexcept { return items.end(); }
    auto                begin           () const noexcept { return items.begin(); }
    auto                end             ()   const noexcept { return items.end(); }

private:
    struct FSlot
    {
        uint32 itemIndex  = 0;
        uint32 generation = 0;
        bool   alive      = false;
    };

    FSlot*              GetSlot         (TPoolHandle<T> handle);

    const FSlot*        GetSlot         (TPoolHandle<T> handle) const;

    TArray<T>      items;       
    TArray<uint32> itemToSlot;
    TArray<FSlot>  slots;       
    TArray<uint32> freeList;    
};

}

#include <Core/Containers/TPool.inl>