#pragma once
#include <filesystem>
#include "ShellFileUtil.h"

namespace fs = std::filesystem;

void ShellFileUtil::deletePrevOutputFile()
{
    if (file_exists(OUTPUT_FILE)) {
        if (deleteFile(OUTPUT_FILE) == DELETE_SUCCESS) {
        }
        else {
            std::cout << "fail to delete " << OUTPUT_FILE << std::endl;
        }
    }
}

void ShellFileUtil::writeOutputFile(std::string& writeString)
{
    const std::size_t MAX_SIZE = 10 * 1024;
    if (isFileTooLarge(OUTPUT_FILE, MAX_SIZE)) {
        renameLogFile();
    }
    std::ofstream out(OUTPUT_FILE, std::ios::app);
    if (out.is_open()) {
        out << writeString << std::endl;
        out.close();
    }
    else {
        std::cout << "fail to create output file";
    }
}

void ShellFileUtil::renameLogFile()
{
    if (!fs::exists(OUTPUT_FILE)) {
        std::cout << "No log file to rename: " << OUTPUT_FILE << std::endl;
        return;
    }

    std::string newName = makeTimestampedFilename();
    fs::path newPath(newName);

    bool untilLogExists = false;
    fs::path existingUntilLog;

    for (const auto& entry : fs::directory_iterator(fs::current_path()))
    {
        if (entry.is_regular_file())
        {
            auto filename = entry.path().filename().string();
            if (filename.size() > 6 &&
                filename.substr(0, 6) == "until_" &&
                entry.path().extension() == ".log")
            {
                untilLogExists = true;
                existingUntilLog = entry.path();
                break;
            }
        }
    }

    if (untilLogExists)
    {
        fs::path zipPath = existingUntilLog;
        zipPath.replace_extension(".zip");

        try {
            fs::rename(existingUntilLog, zipPath);
            std::cout << "Existing until_*.log renamed to: " << zipPath << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to rename existing until_*.log to zip: " << e.what() << std::endl;
            return;
        }
    }

    try {
        fs::rename(OUTPUT_FILE, newPath);
        std::cout << "Log file renamed: " << OUTPUT_FILE << " -> " << newPath << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to rename latest.log: " << e.what() << std::endl;
    }
}

bool ShellFileUtil::isFileTooLarge(const std::string& filename, std::size_t maxSize)
{
    if (!file_exists(filename)) return false;

    return fs::file_size(filename) > maxSize;

}

bool ShellFileUtil::file_exists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}
int ShellFileUtil::deleteFile(const std::string& fileName)
{
    return std::remove(fileName.c_str());
}

std::string ShellFileUtil::makeTimestampedFilename()
{
    time_t now = time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);

    std::ostringstream oss;
    oss << "until_"
        << std::setw(2) << std::setfill('0') << (timeInfo.tm_year % 100)
        << std::setw(2) << std::setfill('0') << (timeInfo.tm_mon + 1)
        << std::setw(2) << std::setfill('0') << timeInfo.tm_mday << "_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_hour << "h_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_min << "m_"
        << std::setw(2) << std::setfill('0') << timeInfo.tm_sec << "s"
        << ".log";

    return oss.str();
}