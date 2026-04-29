#include <Core/Log.h>
#include <chrono>
#include <format>
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
        std::ofstream GLogFile;
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

            const std::filesystem::path logDir = std::filesystem::current_path() / "Saved" / "Logs";
            std::filesystem::create_directories(logDir);

            std::ostringstream oss;
            oss << "Log_" << GetFileTimestamp() << ".txt";
            const std::filesystem::path logFilePath = logDir / oss.str();
            GLogFile.open(logFilePath, std::ios::out | std::ios::app);

            GInitialized = true;
        }

#ifdef _WIN32
        void SetConsoleColor(LogType type)
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

            WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // white
            if (type == LogType::Warning)
                color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // yellow
            else if (type == LogType::Error)
                color = FOREGROUND_RED | FOREGROUND_INTENSITY; // red

            SetConsoleTextAttribute(h, color);
        }

        void ResetConsoleColor()
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
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

        std::ostringstream lineOss;
        lineOss << "[" << GetTimeHHMMSS() << "] [" << ToString(type) << "]: " << message;
        const std::string line = lineOss.str();

#ifdef _WIN32
        SetConsoleColor(type);
#endif
        std::cout << line << '\n';
#ifdef _WIN32
        ResetConsoleColor();
#endif

        if (GLogFile.is_open())
        {
            GLogFile << line << '\n';
            GLogFile.flush();
        }
    }
}
