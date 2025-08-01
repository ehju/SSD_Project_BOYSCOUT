#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <mutex>


class Logger {
private:
    Logger() = default;                       // 생성자를 private으로 막음
    static Logger* instance;                 // 인스턴스 포인터
    static std::mutex mtx;                   // 멀티스레드 안전 보장용
    std::string GetTimestamp();
    void splitFunctionSignature(const std::string& fullFunc, std::string& className, std::string& functionName);

public:
    Logger(const Logger&) = delete;          // 복사 금지
    Logger& operator=(const Logger&) = delete;

    static Logger* getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new Logger();         // 최초 1회만 생성
        }
        return instance;
    }

    void print(const char* functionName, const char* fmt, ...);

};


