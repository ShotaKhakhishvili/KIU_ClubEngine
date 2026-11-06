#include <Core/Types/InputTypes.h>

#include <iterator>

namespace CE
{

namespace
{

constexpr const char* keyNames[] =
{
    "None",

#define CE_KEY_NAME(Name) #Name,
    CE_INPUT_KEYS(CE_KEY_NAME)
#undef CE_KEY_NAME
};

static_assert(std::size(keyNames) == static_cast<size_t>(Count), "Key count must be the sames as key string counts");

}

char ToString(const EKey key)
{
    return *keyNames[static_cast<size_t>(key)];
}

EKey ToKey(const std::string_view key)
{
    for (size_t i = 0; i < std::size(keyNames); ++i)
    {
        if (keyNames[i] == key)
            return static_cast<EKey>(i);
    }

    return None;
}

}