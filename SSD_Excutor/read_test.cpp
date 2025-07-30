#include "gmock/gmock.h"
#include <iostream>
#include <fstream>
#include <string>
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
    SSD ssd;
};

// if read command issued, there must be ssd_output.txt
TEST_F(ReadTestFixture, ReadCommandLeavesOutputfile) {
    Read readCommand;
    int address = 0x0;
    readCommand.execute(address);

    EXPECT_EQ(ssdHelper.isFileExist(OUTPUT_FILE_NAME), true);
}

// read command should return 0x00000000 even if ssd has never been used 
TEST_F(ReadTestFixture, NeverWrittenReadReturnZero) {
    Read readCommand;
    int address = 0x0;
    readCommand.execute(address);

    ssdHelper.resetSSD();

    EXPECT_THAT(readCommand.execute(address), "0x00000000");
}

// if lba 1 is written with specific value, lba 1 read should return exactly same value.
TEST_F(ReadTestFixture, DISABLED_CompareReadResultWithWriteValue) {
    unsigned int address = 0x0;
    unsigned int writeValue = 0x0;

    ssd.writeCommand.execute(address, writeValue);
    ssd.readCommand.execute(address);

    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    
    EXPECT_EQ(readValue, writeValue);
}

// read should return 0x00000000 if non-written lba being read
// write 0x0 and then read 0x1 which has never been written
TEST_F(ReadTestFixture, DISABLED_NonWrittenLBARead) {
    unsigned int writeAddress = 0x0;
    unsigned int readAddress = 0x1;
    unsigned int writeValue = 0x0;

    ssdHelper.resetSSD();
    ssd.writeCommand.execute(writeAddress, writeValue);
    ssd.readCommand.execute(readAddress);

    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);

    EXPECT_EQ(readValue, (unsigned int)0x00000000);
}

// output.txt should contains last read command output which means overwrite should work
TEST_F(ReadTestFixture, DISABLED_OutputShouldContainsOnlyLastReadResult) {
    unsigned int addressOne = 0x1;
    unsigned int addressTwo = 0x2;
    unsigned int addressThree = 0x3;
    unsigned int valueOne = 0x1;
    unsigned int valueTwo = 0x1;
    unsigned int valueThree = 0x1;

    ssdHelper.resetSSD();

    ssd.writeCommand.execute(addressOne, valueOne);
    ssd.writeCommand.execute(addressTwo, valueTwo);
    ssd.writeCommand.execute(addressThree, valueThree);

    ssd.readCommand.execute(addressOne);
    ssd.readCommand.execute(addressTwo);
    ssd.readCommand.execute(addressThree);

    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);

    EXPECT_THAT(readValue, Ne(addressOne));
    EXPECT_THAT(readValue, Ne(addressTwo));
    EXPECT_THAT(readValue, Eq(addressThree));
}

// read with out of range, write ERROR in nand_output.txt
TEST_F(ReadTestFixture, DISABLED_OutOfRangeRead) {
    unsigned int OutOfRangeAddress = 100;

    ssd.readCommand.execute(OutOfRangeAddress);

    EXPECT_THAT(ssdHelper.getReadResultFromFile(), Eq("ERROR"));
}