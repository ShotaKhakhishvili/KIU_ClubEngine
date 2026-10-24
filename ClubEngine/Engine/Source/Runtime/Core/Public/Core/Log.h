#pragma once

#include <format>
#include <string>
#include <string_view>
#include <utility>

namespace CE
{
    enum class ELogType
    {
        Info,
        Warning,
        Error
    };

    void InitializeLogger();
    void LogMessage(ELogType type, const std::string& message);

    template<typename... Args>
    void LogFormat(ELogType type, std::string_view fmt, Args&&... args)
    {
        LogMessage(type, std::vformat(fmt, std::make_format_args(args...)));
    }
}

#define CE_LOG(Type, ...) \
    ::CE::LogFormat(::CE::ELogType::Type, __VA_ARGS__)
    