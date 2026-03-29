#include <Core/Log.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Logger {

    static std::ofstream s_LogFile;

    // Helper to get current time as a string [HH:MM:SS]
    static std::string GetCurrentTimeStr() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm lt;
        #ifdef _WIN32
            localtime_s(&lt, &now_time);
        #else
            lt = *std::localtime(&now_time);
        #endif
        std::stringstream ss;
        ss << "[" << std::put_time(&lt, "%H:%M:%S") << "]";
        return ss.str();
    }

    void Init() {
        // Ensure the directory exists
        std::filesystem::path logPath = "Projects/Saved/Logs";
        if (!std::filesystem::exists(logPath)) {
            std::filesystem::create_directories(logPath);
        }

        // Generate Filename
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm lt;
        #ifdef _WIN32
            localtime_s(&lt, &now_time);
        #else
            lt = *std::localtime(&now_time);
        #endif

        std::stringstream ss;
        ss << "Saved/Logs/Log_" << std::put_time(&lt, "%Y-%m-%d_%H-%M-%S") << ".txt";
        
        // Open the file
        s_LogFile.open(ss.str(), std::ios::out | std::ios::app);
        
        if (s_LogFile.is_open()) {
            s_LogFile << "--- Log Session Started: " << std::put_time(&lt, "%Y-%m-%d %H:%M:%S") << " ---\n";
            s_LogFile.flush();
        }
    }

    void LogInternal(LogType type, const std::string& message) {
        std::string timeStr = GetCurrentTimeStr();
        std::string typeStr;
        int colorCode = 7; // Default White

        // Determine Type String and Color
        switch (type) {
            case LogType::Info:
                typeStr = "[Info]";
                colorCode = 15; // Bright White
                break;
            case LogType::Warning:
                typeStr = "[Warning]";
                colorCode = 14; // Yellow
                break;
            case LogType::Error:
                typeStr = "[Error]";
                colorCode = 12; // Red
                break;
        }

        // Construct final message
        std::string finalMessage = timeStr + " " + typeStr + ": " + message;

        // Print to Console with Colors
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colorCode);
        std::cout << finalMessage << std::endl;
        SetConsoleTextAttribute(hConsole, 7); // Reset to default white
#else
        std::cout << finalMessage << std::endl;
#endif

        // Write to File
        if (s_LogFile.is_open()) {
            s_LogFile << finalMessage << "\n";
            s_LogFile.flush(); // Ensure it writes even if engine crashes later
        }
    }
}