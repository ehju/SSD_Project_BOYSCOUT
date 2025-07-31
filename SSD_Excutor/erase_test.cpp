#include "gmock/gmock.h"

#include "read.h"
#include "write.h"
#include "erase.h"
#include "SSDHelper.cpp"

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

    writeCommand.execute(addressOne, valueOne);
    writeCommand.execute(addressTwo, valueTwo);
    writeCommand.execute(addressThree, valueThree);

    eraseCommand.execute(addressOne, (unsigned int)0x2);
    eraseCommand.execute(addressThree, (unsigned int)0x1);

    unsigned int readValue;
    unsigned int expectErasedValue = static_cast<unsigned int>(0x0);
    readCommand.execute(addressOne);
    readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    EXPECT_EQ(readValue, expectErasedValue);
    
    readCommand.execute(addressTwo);
    readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    EXPECT_EQ(readValue, expectErasedValue);
    
    readCommand.execute(addressThree);
    readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    EXPECT_EQ(readValue, expectErasedValue);
}
