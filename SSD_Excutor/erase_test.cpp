#include "gmock/gmock.h"

#include "read.h"
#include "write.h"
#include "erase.h"
#include "SSDHelper.h"

using namespace testing;

class EraseTestFixture : public Test {
public:
    SSDHelper ssdHelper;
    Read readCommand;
    Write writeCommand;
    Erase eraseCommand;
};


TEST_F(EraseTestFixture, EraseAfterWrite) {
    ssdHelper.resetSSD();

    unsigned int addressOne = 0x1;
    unsigned int addressTwo = 0x2;
    unsigned int addressThree = 0x3;
    unsigned int valueOne = 0x1;
    unsigned int valueTwo = 0x2;
    unsigned int valueThree = 0x3;

    writeCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), addressOne, valueOne });
    writeCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), addressTwo, valueTwo });
    writeCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), addressThree, valueThree });

    eraseCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), addressOne, static_cast<unsigned int>(0x2) });
    eraseCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE), addressThree, static_cast<unsigned int>(0x1) });

    unsigned int readValue;
    unsigned int expectErasedValue = static_cast<unsigned int>(0x0);
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), addressOne, static_cast<unsigned int>(0xFFFFFFFF) });
    readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    EXPECT_EQ(readValue, expectErasedValue);
    
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), addressTwo, static_cast<unsigned int>(0xFFFFFFFF) });
    readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    EXPECT_EQ(readValue, expectErasedValue);
    
    readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), addressThree, static_cast<unsigned int>(0xFFFFFFFF) });
    readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    EXPECT_EQ(readValue, expectErasedValue);
}
