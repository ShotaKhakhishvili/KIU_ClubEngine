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
    explicit TObjectHandle(UObjectHandle inHandle)
        :   handle(inHandle)
    {}

    template<typename U>
        requires std::is_base_of_v<T, U>
    TObjectHandle(const TObjectHandle<U>& other) noexcept
        :   handle(other.GetRaw())
    {}

    bool IsValid() const noexcept
    {
        return handle.IsValid();
    }
    const UObjectHandle& GetRaw() const noexcept
    {
        return handle;
    }

private:
    friend class UObjectRegistry;

    UObjectHandle handle{};
};

}