#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include "ShellLogger.h"

void Logger::print(const char* fullfunctionName, const char* fmt, ...)
{
    char message[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    std::string className, functionName;
    splitFunctionSignature(fullfunctionName, className, functionName);
    std::ostringstream oss;
    oss << className << "." << functionName << "()";

    std::cout << GetTimestamp() << " " << std::left << std::setw(30) << oss.str() << " : " << message << std::endl;

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