#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
class SSDHelper {
public:
    bool isFileExist(const std::string& fileName);
    std::string getReadResultFromFile();
    void resetSSD();

private:
    bool file_exists(const std::string& filename);
    const std::string SSD_FILE_NAME = "ssd_nand.txt";
    const std::string OUTPUT_FILE_NAME = "ssd_output.txt";
};