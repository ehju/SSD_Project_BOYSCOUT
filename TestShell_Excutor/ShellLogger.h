#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <mutex>


class Logger {
private:
    Logger() = default;                       // �����ڸ� private���� ����
    static Logger* instance;                 // �ν��Ͻ� ������
    static std::mutex mtx;                   // ��Ƽ������ ���� �����
    std::string GetTimestamp();
    void splitFunctionSignature(const std::string& fullFunc, std::string& className, std::string& functionName);

public:
    Logger(const Logger&) = delete;          // ���� ����
    Logger& operator=(const Logger&) = delete;

    static Logger* getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new Logger();         // ���� 1ȸ�� ����
        }
        return instance;
    }

    void print(const char* functionName, const char* fmt, ...);

};


