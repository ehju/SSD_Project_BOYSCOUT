#include "gmock/gmock.h"
#include <iostream>

#include <string>
#include "read.cpp"
#include "write.cpp"

#include "SSDHelper.cpp"
using namespace testing;

const static std::string SSD_FILE_NAME = "ssd_nand.txt";
const static std::string OUTPUT_FILE_NAME = "ssd_output.txt";

class ReadTestFixture : public Test {
public:
    SSDHelper ssdHelper;
    Read readCommand;
    Write writeCommand;
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
    ssdHelper.resetSSD();

    unsigned int address = 0x0;
    readCommand.execute(address);

    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);

    EXPECT_THAT(readValue, (unsigned int)0x00000000);
}

// if lba 1 is written with specific value, lba 1 read should return exactly same value.
TEST_F(ReadTestFixture, CompareReadResultWithWriteValue) {
    unsigned int address = 0x1;
    unsigned int writeValue = 0x1;

    writeCommand.execute(address, writeValue);
    readCommand.execute(address);

    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    
    EXPECT_EQ(readValue, writeValue);
}

// read should return 0x00000000 if non-written lba being read
// write 0x0 and then read 0x1 which has never been written
TEST_F(ReadTestFixture, NonWrittenLBARead) {
    unsigned int writeAddress = 0x0;
    unsigned int readAddress = 0x1;
    unsigned int writeValue = 0x1;

    ssdHelper.resetSSD();
    writeCommand.execute(writeAddress, writeValue);
    readCommand.execute(readAddress);

    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);

    EXPECT_EQ(readValue, (unsigned int)0x00000000);
}

// output.txt should contains last read command output which means overwrite should work
TEST_F(ReadTestFixture, OutputShouldContainsOnlyLastReadResult) {
    unsigned int addressOne = 0x1;
    unsigned int addressTwo = 0x2;
    unsigned int addressThree = 0x3;
    unsigned int valueOne = 0x1;
    unsigned int valueTwo = 0x2;
    unsigned int valueThree = 0x3;

    ssdHelper.resetSSD();

    writeCommand.execute(addressOne, valueOne);
    writeCommand.execute(addressTwo, valueTwo);
    writeCommand.execute(addressThree, valueThree);

    readCommand.execute(addressOne);
    readCommand.execute(addressTwo);
    readCommand.execute(addressThree);

    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);

    EXPECT_THAT(readValue, Ne(valueOne));
    EXPECT_THAT(readValue, Ne(valueTwo));
    EXPECT_THAT(readValue, Eq(valueThree));
}