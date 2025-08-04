#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include "ShellLogger.h"
#include "ShellFileUtil.h"

void Logger::print(const char* fullfunctionName, const char* fmt, ...)
{
    char message[MESSAGE_BUFFER_MAX];
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    std::string className, functionName;
    splitFunctionSignature(fullfunctionName, className, functionName);
    std::ostringstream oss;
    oss << GetTimestamp() << " " << std::left << std::setw(ALIGN_SPACE) << (className + "." + functionName + "( )")
     << " : " << message;
    std::string outputLine = oss.str();

   // std::cout << outputLine << std::endl;

    ShellFileUtil LogFile;
    LogFile.writeOutputFile(outputLine);
}

std::string Logger::GetTimestamp()
{
    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);

    std::ostringstream oss;
    oss << '['
        << std::setw(2) << std::setfill('0') << (timeInfo.tm_year % 100) << '.'
        << std::setw(2) << std::setfill('0') << (timeInfo.tm_mon + 1) << '.'
        << std::setw(2) << std::setfill('0') << timeInfo.tm_mday << ' '
        << std::setw(2) << std::setfill('0') << timeInfo.tm_hour << ':'
        << std::setw(2) << std::setfill('0') << timeInfo.tm_min << ']';

    return oss.str();
}

void Logger::splitFunctionSignature(const std::string& fullFunc, std::string& className, std::string& functionName)
{
    size_t pos = fullFunc.rfind("::");
    if (pos != std::string::npos)
    {
        className = fullFunc.substr(0, pos);
        functionName = fullFunc.substr(pos + 2);
    }
    else {
        className = "NoClass";
        functionName = fullFunc;
    }
}

Logger* Logger::instance = nullptr;
std::mutex Logger::mtx;
