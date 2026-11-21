#pragma once

#include <Core/Log.h>

#include <charconv>
#include <type_traits>
#include <string>

namespace CE
{

namespace ConfigDetail
{

template<typename T>
bool ParseValue(const std::string& str, T& out)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        out = str;
        return true;
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        if (str == "true"  || str == "True"  || str == "1") { out = true;  return true; }
        if (str == "false" || str == "False" || str == "0") { out = false; return true; }
        return false;
    }
    else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
    {
        const char* end = str.data() + str.size();
        const auto [ptr, ec] = std::from_chars(str.data(), end, out);
        return ec == std::errc{} && ptr == end;
    }
    else
    {
        static_assert(!sizeof(T), "ConfigSystem::Get: unsupported type");
        return false;
    }
}

}
template <typename T>
T ConfigSystem::Get(const std::string& section, const std::string& key) const
{
    const auto secIt = data.find(section);
    if (secIt == data.end())
    {
        CE_LOG(Error, "Config: missing section [{}]", section);
        return T{};
    }

    const auto keyIt = secIt->second.find(key);
    if (keyIt == secIt->second.end())
    {
        CE_LOG(Error, "Config: missing key [{}] {}", section, key);
        return T{};
    }

    T value{};
    if (!ConfigDetail::ParseValue(keyIt->second, value))
    {
        CE_LOG(Error, "Config: [{}] {}={} is not a valid value", section, key, keyIt->second);
        return T{};
    }

    return value;
}

}