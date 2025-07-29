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
// read ��� �����ߴٸ�, ssd_output.txt�� �����ؾ��Ѵ�.
TEST_F(ReadTestFixture, ReadCommandLeavesOutputfile) {
    SSD ssd;
    int address = 0x0;
    ssd.readCommand.execute(address);
    EXPECT_THAT(fileChecker.isFileExist("ssd_output.txt"), Eq(true));
}

// if don't exist nand.txt, read command output will be 
// write������ ���� �� ���� ���� ssd�� read�ϴ��� output file�� 0x0000000�� ������ �Ѵ�.
TEST_F(ReadTestFixture, NeverWrittenReadReturnZero) {
    SSD ssd;
    int address = 0x1;
    ssd.readCommand.execute(address);
    EXPECT_THAT(fileChecker.fileAddressRead(address), "0x0000000");
}

//if lba 1 is written, lba 1 read
// write�� �����ϰ� ������ �ּҸ� �о��� ��, write�� ���� read�� ���� ������ Ȯ��

// unwritten lba read return 0x00000000
// write�� ������ ��, write���� ���� ���� read ���� �� 0�� ������ �Ѵ�.

// multi read, and output.txt return last read command output
// read�� ���� �� �������� ��, ������ read�� �ش��ϴ� output�� �����־�� �Ѵ�.


// read with out of range, write ERROR in nand_output.txt
// ���� ���� �ּҸ� ������ �� ������ output ���Ͽ� ERROR�� ������ �Ѵ�.