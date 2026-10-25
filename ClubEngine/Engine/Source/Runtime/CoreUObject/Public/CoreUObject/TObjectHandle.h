#pragma once

#include <CoreUObject/UObjectHandle.h>

#include <type_traits>

namespace CE
{

template<typename T>
class TObjectHandle
{
public:
    TObjectHandle() = default;
    explicit TObjectHandle(UObjectHandle inHandle);

    template<typename U>
        requires std::is_base_of_v<T, U>
    TObjectHandle(const TObjectHandle<U>& other) noexcept;

    bool IsValid() const noexcept;
    const UObjectHandle& GetRaw() const noexcept;

    bool operator==(const TObjectHandle& object_handle) const;
    void Invalidate();

    uint32 GetRegistryID() const noexcept;

private:
    friend class UObjectRegistry;

    UObjectHandle handle{};
};


}

#include "TObjectHandle.inl"