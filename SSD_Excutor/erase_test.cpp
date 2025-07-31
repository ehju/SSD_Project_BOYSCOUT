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

    unsigned int addressOne = 0x1;
    unsigned int addressTwo = 0x2;
    unsigned int addressThree = 0x3;
    unsigned int valueOne = 0x1;
    unsigned int valueTwo = 0x2;
    unsigned int valueThree = 0x3;
    unsigned int readValue;
    unsigned int expectErasedValue = static_cast<unsigned int>(0x0);

    void writeNonZeroValueFromLBA1To3()
    {
        writeCommand.execute(addressOne, valueOne);
        writeCommand.execute(addressTwo, valueTwo);
        writeCommand.execute(addressThree, valueThree);
    }

    unsigned long getU_INTValueFromOutputFile()
    {
        return std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
    }

    void checkZero(unsigned int address)
    {
        readCommand.execute(address);
        readValue = std::stoul(ssdHelper.getReadResultFromFile(), nullptr, 16);
        EXPECT_EQ(readValue, expectErasedValue);
    }

    void checkAddressValue(unsigned int address, unsigned int expectedValue)
    {
        readCommand.execute(address);
        readValue = getU_INTValueFromOutputFile();
        EXPECT_EQ(readValue, expectedValue);
    }
};

TEST_F(EraseTestFixture, EraseAfterWrite) {
    ssdHelper.resetSSD();

    // write non-zero value on LBA 1,2,3
    writeNonZeroValueFromLBA1To3();

    // check write value
    checkAddressValue(addressOne,valueOne);
    checkAddressValue(addressTwo,valueTwo);
    checkAddressValue(addressThree,valueThree);

    // erase
    eraseCommand.execute(addressOne, (unsigned int)0x2);
    eraseCommand.execute(addressThree, (unsigned int)0x1);

    // check erased value
    checkZero(addressOne);
    checkZero(addressTwo);
    checkZero(addressThree);
}







