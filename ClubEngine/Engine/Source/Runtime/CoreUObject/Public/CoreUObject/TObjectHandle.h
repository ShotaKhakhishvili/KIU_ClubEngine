#pragma once

#include <CoreUObject/UObjectHandle.h>

template<typename T>
class TObjectHandle
{
public:
    TObjectHandle() = default;
    explicit TObjectHandle(UObjectHandle inHandle)
        :   handle(inHandle)
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