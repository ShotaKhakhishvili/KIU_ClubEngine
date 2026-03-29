#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <sstream>

enum class LogType {
    Info,
    Warning,
    Error
};

namespace Logger {

    void LogInternal(LogType type, const std::string& message);

    // Helper to convert anything to string
    template <typename T>
    std::string ToString(T value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    // Base case for recursion
    inline void FormatString(std::string& s, size_t pos) {}

    // Recursive template to replace {} with arguments
    template <typename T, typename... Args>
    void FormatString(std::string& s, size_t pos, T first, Args... args) {
        pos = s.find("{}", pos);
        if (pos != std::string::npos) {
            std::string val = ToString(first);
            s.replace(pos, 2, val);
            FormatString(s, pos + val.length(), args...);
        }
    }

    // The function the macro calls
    template <typename... Args>
    void Log(LogType type, std::string_view format, Args... args) {
        std::string message(format);
        FormatString(message, 0, args...); // Replace {} with actual values
        LogInternal(type, message); // Send the finished string to Log.cpp
    }
}

// The Macro
#define CE_LOG(type, format, ...) Logger::Log(LogType::type, format, ##__VA_ARGS__)