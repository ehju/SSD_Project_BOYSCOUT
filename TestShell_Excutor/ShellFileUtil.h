#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	
class ShellFileUtil {
public:
    static void deletePrevOutputFile();
    static void writeOutputFile(std::string& writeString);
    static void renameLogFile();

private:
    static bool isFileTooLarge(const std::string& filename, std::size_t maxSize);
    static bool file_exists(const std::string& filename);
    static int deleteFile(const std::string& fileName);
    static std::string makeTimestampedFilename();
    inline static const int DELETE_SUCCESS = 0;
    inline static const std::string OUTPUT_FILE = "latest.log";

};