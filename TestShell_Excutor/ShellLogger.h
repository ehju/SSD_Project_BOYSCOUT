#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <string>

class Logger {
public:
    Logger();
    void print(const char* functionName, const char* fmt, ...);

private:
    std::string GetTimestamp();
    void splitFunctionSignature(const std::string& fullFunc, std::string& className, std::string& functionName);
};