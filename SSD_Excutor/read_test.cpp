#include "gmock/gmock.h"
#include <iostream>
#include <fstream>
#include "ssd.cpp"

using namespace testing;

const std::string SSD_FILE_NAME = "ssd_nand.txt";
const std::string OUTPUT_FILE_NAME = "ssd_output.txt";

class SSDHelper {
public:
    bool isFileExist(const std::string& fileName) {
        std::ifstream file(fileName);
        return file.is_open();
    }
    std::string getReadResultFromFile() {
        std::ifstream file(OUTPUT_FILE_NAME);
        std::string line;
        if (file.is_open())
        {
            getline(file, line);
            return line;
        }
        return "";
    }

    void resetSSD() {
        // clear SSD_FILE_
        // clear OUTPUT_FILE_
    }
};

class ReadTestFixture : public Test {
public:
    SSDHelper ssdHelper;
;
};

// if read command issued, there must be ssd_output.txt
TEST_F(ReadTestFixture, ReadCommandLeavesOutputfile) {
    SSD ssd;
    int address = 0x0;
    ssd.readCommand.execute(address);

    EXPECT_EQ(ssdHelper.isFileExist(OUTPUT_FILE_NAME), true);
}

// read command should return 0x00000000 even if ssd has never been used 
TEST_F(ReadTestFixture, NeverWrittenReadReturnZero) {
    SSD ssd;
    int address = 0x0;
    ssd.readCommand.execute(address);

    // reset SSD to delete all txt files
    ssdHelper.resetSSD();

    EXPECT_THAT(ssd.readCommand.execute(address), "0x00000000");
}

// if lba 1 is written with specific value, lba 1 read should return exactly same value.

// read should return 0x00000000 if non-written lba being read
// write 0x0 and then read 0x1 which has never been written

// output.txt should contains last read command output which means overwrite should work


// read with out of range, write ERROR in nand_output.txt
