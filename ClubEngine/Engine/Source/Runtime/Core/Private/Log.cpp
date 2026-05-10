#include <Core/Log.h>
#include <chrono>
#include <cstdlib>      
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

            std::filesystem::path currentPath = std::filesystem::current_path();
            std::filesystem::path projectLogDir = currentPath / "Saved" / "Logs";
            std::filesystem::create_directories(projectLogDir);
            GProjectLogFile.open(projectLogDir / fileName, std::ios::out | std::ios::app);

            std::filesystem::path engineDir;

#ifdef _MSC_VER
            char* envPath = nullptr;
            size_t len;
            _dupenv_s(&envPath, &len, "CLUBENGINE_ROOT");
            if (envPath != nullptr) {
                engineDir = envPath;
                free(envPath); 
            }
#else
            const char* envPath = std::getenv("CLUBENGINE_ROOT");
            if (envPath != nullptr) {
                engineDir = envPath;
            }
#endif

            if (!engineDir.empty()) 
            {
                std::filesystem::path engineLogDir = engineDir / "Saved" / "Logs";
                
                std::error_code ec;
                std::filesystem::create_directories(engineLogDir, ec);

                if (!ec) {
                    GEngineLogFile.open(engineLogDir / fileName, std::ios::out | std::ios::app);
                } else {
                    std::cerr << "Failed to create Engine Log Directory: " << ec.message() << '\n';
                }
            }
            else
            {
                std::cerr << "Warning: CLUBENGINE_ROOT environment variable is not set. Engine logs will not be saved.\n";
            }

            GInitialized = true;
        }

#ifdef _WIN32
        void SetConsoleColor(LogType type)
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; 
            
            if (type == LogType::Warning)
                color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; 
            else if (type == LogType::Error)
                color = FOREGROUND_RED | FOREGROUND_INTENSITY; 

            SetConsoleTextAttribute(h, color);
        }

        void ResetConsoleColor()
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
#else
        const char* GetAnsiColor(LogType type)
        {
            if (type == LogType::Warning) return "\033[33m"; 
            if (type == LogType::Error)   return "\033[31m"; 
            return "\033[37m"; 
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

        std::ostringstream lineOss;
        lineOss << "[" << GetTimeHHMMSS() << "] [" << ToString(type) << "]: " << message;
        const std::string line = lineOss.str();

#ifdef _WIN32
        SetConsoleColor(type);
        std::cout << line << '\n';
        ResetConsoleColor();
#else
        std::cout << GetAnsiColor(type) << line << GetAnsiReset() << '\n';
#endif

        if (GProjectLogFile.is_open())
        {
            GProjectLogFile << line << '\n';
            GProjectLogFile.flush();
        }

        if (GEngineLogFile.is_open())
        {
            GEngineLogFile << line << '\n';
            GEngineLogFile.flush();
        }
    }
}