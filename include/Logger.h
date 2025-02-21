#pragma once
#include <string>
#include <chrono>

namespace cl {
    const unsigned int LOG_LEVEL_INFO = 1;
    const unsigned int LOG_LEVEL_DEBUG = 2;
    const unsigned int LOG_LEVEL_ERROR = 4;

    const unsigned int LOG_LEVEL_ALL = LOG_LEVEL_INFO | LOG_LEVEL_DEBUG | LOG_LEVEL_ERROR;

    class Logger {
    protected:
        int result;
        std::string info, debug, error;
        std::chrono::time_point<std::chrono::high_resolution_clock> init_time;

    public:
        Logger() : result(0), init_time(std::chrono::high_resolution_clock::now()) {};
        ~Logger() {};
        
        inline int getResult() { return result; };
        inline bool checkResult(int target) { return result == target; };
        inline std::string getInfo() { return info; };
        inline std::string getDebug() { return debug; };
        inline std::string getError() { return error; };

        std::string getMessage(unsigned int level);
        void logInfo(std::string message);
        void logDebug(std::string message);
        void logError(std::string message);
        std::string getResultString();
    };
}