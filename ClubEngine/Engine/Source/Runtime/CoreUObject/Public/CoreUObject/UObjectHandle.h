#pragma once

#include <Core/Types/CoreTypes.h>

#include <cstdint>
#include <limits>

namespace CE
{

class UObjectHandle
{
public:
    bool IsValid() const noexcept;
    void Invalidate() noexcept;

    uint32 GetRegistryID() const noexcept;

    bool operator==(const UObjectHandle& object_handle) const noexcept = default;

private:
    friend class UObjectRegistry;

    uint32 index = std::numeric_limits<uint32_t>::max();
    uint32 generation = 0;
    uint32 registryId = 1;
};

}