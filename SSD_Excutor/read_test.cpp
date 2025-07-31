#include "gmock/gmock.h"
#include <iostream>

#include <string>
#include "read.h"
#include "write.h"

#include "SSDHelper.h"
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
    unsigned int address = 0x0;
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), address, static_cast<unsigned int>(0xFFFFFFFF) });

    EXPECT_EQ(ssdHelper.isFileExist(OUTPUT_FILE_NAME), true);
}

// read command should return 0x00000000 even if ssd has never been used 
TEST_F(ReadTestFixture, NeverWrittenReadReturnZero) {    
    ssdHelper.resetSSD();

    unsigned int address = 0x0;
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), address, static_cast<unsigned int>(0xFFFFFFFF) });


    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);

    EXPECT_THAT(readValue, (unsigned int)0x00000000);
}

// if lba 1 is written with specific value, lba 1 read should return exactly same value.
TEST_F(ReadTestFixture, CompareReadResultWithWriteValue) {
    unsigned int address = 0x1;
    unsigned int writeValue = 0x1;

    writeCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), address, writeValue });
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), address, static_cast<unsigned int>(0xFFFFFFFF) });
    
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
    writeCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), writeAddress, writeValue });
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), readAddress, static_cast<unsigned int>(0xFFFFFFFF) });

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

    writeCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), addressOne, valueOne });
    writeCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), addressTwo, valueTwo });
    writeCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), addressThree, valueThree });
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), addressOne, static_cast<unsigned int>(0xFFFFFFFF) });
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), addressTwo, static_cast<unsigned int>(0xFFFFFFFF) });
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), addressThree, static_cast<unsigned int>(0xFFFFFFFF) });

    unsigned int readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);

    EXPECT_THAT(readValue, Ne(valueOne));
    EXPECT_THAT(readValue, Ne(valueTwo));
    EXPECT_THAT(readValue, Eq(valueThree));
}