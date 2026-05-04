#include <Core/Log.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace CE
{
    namespace
    {
        std::mutex GLogMutex;
        std::ofstream GProjectLogFile;
        std::ofstream GEngineLogFile;
        bool GInitialized = false;

        std::string GetTimeHHMMSS()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::tm localTm{};

#ifdef _WIN32
            localtime_s(&localTm, &t);
#else
            localtime_r(&t, &localTm);
#endif

            std::ostringstream oss;
            oss << std::put_time(&localTm, "%H:%M:%S");
            return oss.str();
        }

        std::string GetFileTimestamp()
        {
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);
            std::tm localTm{};

#ifdef _WIN32
            localtime_s(&localTm, &t);
#else
            localtime_r(&t, &localTm);
#endif

            std::ostringstream oss;
            oss << std::put_time(&localTm, "%Y-%m-%d_%H-%M-%S");
            return oss.str();
        }

        const char* ToString(LogType type)
        {
            switch (type)
            {
            case LogType::Info:    return "Info";
            case LogType::Warning: return "Warning";
            case LogType::Error:   return "Error";
            default:               return "Unknown";
            }
        }

        void EnsureInitializedUnlocked()
        {
            if (GInitialized)
                return;

            std::string fileName = "Log_" + GetFileTimestamp() + ".txt";

            // 1. Setup Project Log Location:
            std::filesystem::path currentPath = std::filesystem::current_path();
            std::filesystem::path projectLogDir = currentPath / "Saved" / "Logs";
            std::filesystem::create_directories(projectLogDir);
            GProjectLogFile.open(projectLogDir / fileName, std::ios::out | std::ios::app);

            // 2. Setup Engine Log Location:
            std::filesystem::path engineDir;
            if (std::filesystem::exists(currentPath.parent_path().parent_path().parent_path().parent_path().parent_path())) {
                engineDir = currentPath.parent_path().parent_path().parent_path().parent_path().parent_path();
            }

            if (!engineDir.empty()) {
                std::filesystem::path engineLogDir = engineDir / "Saved" / "Logs";
                std::filesystem::create_directories(engineLogDir);
                GEngineLogFile.open(engineLogDir / fileName, std::ios::out | std::ios::app);
            }

            GInitialized = true;
        }

        // --- Console Color Management ---
#ifdef _WIN32
        void SetConsoleColor(LogType type)
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White (Info)
            
            if (type == LogType::Warning)
                color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // Yellow
            else if (type == LogType::Error)
                color = FOREGROUND_RED | FOREGROUND_INTENSITY; // Red

            SetConsoleTextAttribute(h, color);
        }

        void ResetConsoleColor()
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
#else
        // Linux/Unix/MSYS2 ANSI Color codes
        const char* GetAnsiColor(LogType type)
        {
            if (type == LogType::Warning) return "\033[33m"; // Yellow
            if (type == LogType::Error)   return "\033[31m"; // Red
            return "\033[37m"; // White (Info)
        }
        const char* GetAnsiReset() { return "\033[0m"; }
#endif
    }

    void InitializeLogger()
    {
        std::lock_guard<std::mutex> lock(GLogMutex);
        EnsureInitializedUnlocked();
    }

    void LogMessage(LogType type, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(GLogMutex);
        EnsureInitializedUnlocked();

        // Format: [HH:MM:SS] [LogType]: LogString
        std::ostringstream lineOss;
        lineOss << "[" << GetTimeHHMMSS() << "] [" << ToString(type) << "]: " << message;
        const std::string line = lineOss.str();

        // Print to Terminal with colors
#ifdef _WIN32
        SetConsoleColor(type);
        std::cout << line << '\n';
        ResetConsoleColor();
#else
        std::cout << GetAnsiColor(type) << line << GetAnsiReset() << '\n';
#endif

        // Write to Project Log File
        if (GProjectLogFile.is_open())
        {
            GProjectLogFile << line << '\n';
            GProjectLogFile.flush();
        }

        // Write to Engine Log File
        if (GEngineLogFile.is_open())
        {
            GEngineLogFile << line << '\n';
            GEngineLogFile.flush();
        }
    }
}