#pragma once

#include <cstdint>
#include <limits>

template<typename T>
class Handle
{
public:
    bool IsValid() const
    {
        return index != std::numeric_limits<uint32_t>::max();
    }

    void Invalidate()
    {
        index = std::numeric_limits<uint32_t>::max();
        generation = 0;
    }

private:
    template<typename>
    friend class AssetManager;
    
    uint32_t index = std::numeric_limits<uint32_t>::max();
    uint32_t generation = 0;
};