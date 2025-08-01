#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "command_parser.h"
#include <filesystem>

namespace fs = std::filesystem;

class SSDHelper {
public:
    bool isFileExist(const std::string& fileName);
    std::string getReadResultFromFile();
    void resetSSD();
    std::string makeExpectedNandString(unsigned int expectedLba, unsigned int expectedValue);
    std::string directAccessNand(unsigned int lba);
    std::string makeCommandBufferName(unsigned int index, CommandInfo cmdInfo);
    void makeCommandBuffer(unsigned int index, CommandInfo cmdInfo);
    std::string getBufferName(unsigned int bufferIndex);

private:
    bool file_exists(const std::string& filename);
    const std::string SSD_FILE_NAME = "ssd_nand.txt";
    const std::string OUTPUT_FILE_NAME = "ssd_output.txt";
    const fs::path BUFFER_SPACE_NAME  = fs::current_path() / "buffer";
};