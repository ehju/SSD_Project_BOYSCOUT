#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <mutex>


class Logger {
private:
    Logger() = default;
    static Logger* instance;
    static std::mutex mtx;
    const  unsigned int ALIGN_SPACE = 45;
    static constexpr unsigned int MESSAGE_BUFFER_MAX = 1024;
    std::string GetTimestamp();
    void splitFunctionSignature(const std::string& fullFunc, std::string& className, std::string& functionName);

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger* getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    void print(const char* functionName, const char* fmt, ...);

};


