#pragma once

#include <format>
#include <string>
#include <string_view>
#include <utility>

namespace CE
{
    enum class LogType
    {
        Info,
        Warning,
        Error
    };

    void InitializeLogger();
    void LogMessage(LogType type, const std::string& message);

    template<typename... Args>
    inline void LogFormat(LogType type, std::string_view fmt, Args&&... args)
    {
        LogMessage(type, std::vformat(fmt, std::make_format_args(args...)));
    }
}

#define CE_LOG(Type, ...) \
    ::CE::LogFormat(::CE::LogType::Type, __VA_ARGS__)
    