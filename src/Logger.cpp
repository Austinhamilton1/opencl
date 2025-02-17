#include <ctime>

#include "Logger.h"

std::string Logger::getMessage(unsigned int level) {
    std::string message;

    //get info level messages
    if((level | LOG_LEVEL_INFO) > 0) {
        message += "INFO\n";
        message += getInfo() + "\n";
    }

    //get debug level messages
    if((level | LOG_LEVEL_DEBUG) > 0) {
        message += "DEBUG\n";
        message += getDebug() + "\n";
    }

    //get error level messages
    if((level | LOG_LEVEL_ERROR) > 0) {
        message += "ERROR\n";
        message += getError() + "\n";
    }

    return message;
}

void Logger::logInfo(std::string message) {
    //add the time and message to info
    auto now = std::chrono::high_resolution_clock::now();
    auto duration  = std::chrono::duration_cast<std::chrono::microseconds>(now - init_time);

    //create the line
    std::string infoLine = "INFO: (";
    infoLine += std::to_string(duration.count());
    infoLine += ") - " + message;
    infoLine += "\n";
    info += infoLine;
}

void Logger::logDebug(std::string message) {
    //add the time and message to debug
    auto now = std::chrono::high_resolution_clock::now();
    auto duration  = std::chrono::duration_cast<std::chrono::microseconds>(now - init_time);

    //create the line
    std::string debugLine = "DEBUG: (";
    debugLine += std::to_string(duration.count());
    debugLine += ") - " + message;
    debugLine += "\n";
    debug += debugLine;
}

void Logger::logError(std::string message) {
    //add the time and message to error
    auto now = std::chrono::high_resolution_clock::now();
    auto duration  = std::chrono::duration_cast<std::chrono::microseconds>(now - init_time);

    //create the line
    std::string errorLine = "ERROR: (";
    errorLine += std::to_string(duration.count());
    errorLine += ") - " + message;
    errorLine += "\n";
    error += errorLine;
}