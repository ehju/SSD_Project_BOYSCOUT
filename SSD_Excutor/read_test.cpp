#include "gmock/gmock.h"
#include <iostream>

#include "ssd.cpp"
using namespace testing;

class FileChecker {
public:
    bool isFileExist(std::string fileName) {
        return true;
    }
    std::string fileAddressRead(int address) {
        return "0x0000000";
    }
};

class ReadTestFixture : public Test {
public:
    FileChecker fileChecker;
};

// if 1 read command issued, exist ssd_output.txt
// read 명령 실행했다면, ssd_output.txt가 존재해야한다.
TEST_F(ReadTestFixture, ReadCommandLeavesOutputfile) {
    SSD ssd;
    int address = 0x0;
    ssd.readCommand.execute(address);
    EXPECT_THAT(fileChecker.isFileExist("ssd_output.txt"), Eq(true));
}

// if don't exist nand.txt, read command output will be 
// write된적이 없는 한 번도 없는 ssd를 read하더라도 output file에 0x0000000이 찍혀야 한다.
TEST_F(ReadTestFixture, NeverWrittenReadReturnZero) {
    SSD ssd;
    int address = 0x1;
    ssd.readCommand.execute(address);
    EXPECT_THAT(fileChecker.fileAddressRead(address), "0x0000000");
}

//if lba 1 is written, lba 1 read
// write을 수행하고 동일한 주소를 읽었을 때, write한 값과 read한 값이 같은지 확인

// unwritten lba read return 0x00000000
// write을 수행한 뒤, write하지 않은 곳을 read 했을 때 0이 읽혀야 한다.

// multi read, and output.txt return last read command output
// read를 여러 번 수행했을 때, 마지막 read에 해당하는 output이 남아있어야 한다.


// read with out of range, write ERROR in nand_output.txt
// 범위 밖의 주소를 읽으려 할 때에는 output 파일에 ERROR가 찍혀야 한다.