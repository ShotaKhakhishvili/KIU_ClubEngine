#pragma once

#include <unordered_map>

namespace CE
{
    // Temporarily cheating, before the implementation is done
    template<typename T, typename U>
    using TMap = std::unordered_map<T,U>;
}