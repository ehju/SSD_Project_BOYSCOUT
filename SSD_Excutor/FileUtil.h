#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	
class FileUtil {
public:
    static void deletePrevOutputFile();
    static void writeOutputFile(std::string& writeString);

private:
    static bool file_exists(const std::string& filename);
    static int deleteFile(const std::string& fileName);
    static const int DELETE_SUCCESS = 0;
};