#include "gmock/gmock.h"
#include "TS_function.cpp"
#include <vector>

using namespace testing;
using std::vector;

#define REAL_DEBUG 0
class MockSSD : public iTS_SSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(bool, write, (int lba, unsigned int data), (override));
};

class SSDFixture : public Test {
public:
	MockSSD ssd;
	TS_function shell{ &ssd };
	int lba = 0;
	unsigned int data = 0x12345678;
};

TEST_F(SSDFixture, WriteBasic) {

	EXPECT_CALL(ssd, write(lba, data))
		.Times(1)
		.WillOnce(Return(true));
		
	EXPECT_EQ(true, shell.write(lba,data));
}

// SSD Read function should be called
TEST_F(SSDFixture, ReadMockTest) {
	EXPECT_CALL(ssd, read(lba))
	    .Times(1);

	unsigned int ret = shell.read(lba);
}

// if invalid LBA range -> throw exception
TEST_F(SSDFixture, ReadMockTestLbaException1) {
 
	lba = -1;

	EXPECT_THROW(shell.read(lba), std::exception);
}
TEST_F(SSDFixture, ReadMockTestLbaException2) {

	lba = 100;
	EXPECT_THROW(shell.read(lba), std::exception);
}

// Read Test state test
TEST_F(SSDFixture, ReadMockResultTest) {

	lba = 10;

	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillOnce(testing::Return(1000));

	unsigned int ret = shell.read(lba);

	EXPECT_EQ(ret, 1000);
}

TEST_F(SSDFixture, NoWriteOutOfRangeLBA1) {
    lba = 100;

	EXPECT_CALL(ssd, write(lba, data))
		.Times(0);
	shell.write(lba, data);
}

TEST_F(SSDFixture, NoWriteOutOfRangeLBA2) {
    lba = -2;

	EXPECT_CALL(ssd, write(lba, data))
		.Times(0);
	shell.write(lba, data);
}

TEST_F(SSDFixture, ReadFullReadTest) {

	vector<unsigned int> result;
	EXPECT_CALL(ssd, read(_))
		.Times(100);

	result = shell.fullread();
}

TEST_F(SSDFixture, ReadFullReadTestExpectedReturn) {
	unsigned int data = 0x00000000;
	
	vector<unsigned int> expected_result;
	vector<unsigned int> result;
	for (int lba = 0; lba < 100; lba++) {
		expected_result.push_back(data);
		EXPECT_CALL(ssd, read(lba))
			.Times(1)
			.WillRepeatedly(Return(data));
		data++;
	}

	result = shell.fullread();
	EXPECT_EQ(expected_result, result);
}

TEST_F(SSDFixture, FullWriteNormal) {
	data = 0xABCDFFFF;
	EXPECT_CALL(ssd, write(_, data))
		.Times(100)
		.WillRepeatedly(Return(true));

	EXPECT_EQ(true, shell.fullwrite(data));
}

TEST_F(SSDFixture, FullWriteFail) {
	data = 0xABCDFFFF;
	EXPECT_CALL(ssd, write(_, data))
		.Times(5)
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	EXPECT_EQ(false, shell.fullwrite(data));
}

TEST_F(SSDFixture, ReadCompareCallSSDRead) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;
	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(true, shell.readCompare(lba, readData));
}

TEST_F(SSDFixture, ReadCompareDifferentDataFail) {
	
	unsigned int writtenData = 0x12345678;
	unsigned int readData = 0x87654321;
	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(false, shell.readCompare(lba, writtenData));
}


TEST_F(SSDFixture, FullWriteAndReadCompareShouldCallFullRangeSSDcommand) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;
	EXPECT_CALL(ssd, write(_, writtenData))
		.Times(100)
		.WillRepeatedly(Return(true));
	
	EXPECT_CALL(ssd, read(_))
		.Times(100)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(true, shell.fullWriteAndReadCompare());
}

TEST_F(SSDFixture, FullWriteAndReadCompare_WriteFail) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;

	EXPECT_CALL(ssd, write(_, _))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	EXPECT_CALL(ssd, read(_))
		.WillRepeatedly(Return(writtenData));
	EXPECT_EQ(false, shell.fullWriteAndReadCompare());
}

TEST_F(SSDFixture, FullWriteAndReadCompare_ReadFail) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = 0x87654321;

	EXPECT_CALL(ssd, write(_, _))
		.WillRepeatedly(Return(true));

	EXPECT_CALL(ssd, read(_))
		.WillOnce(Return(writtenData))
		.WillRepeatedly(Return(readData));
	EXPECT_EQ(false, shell.fullWriteAndReadCompare());
}

#if REAL_DEBUG
TEST_F(SSDFixture, SSDExWrite_Normal) {
	SSDExecutor ssde;
	TS_function shell(&ssde);
	bool expected = true;

	EXPECT_EQ(expected, shell.write(lba,data));
}

TEST_F(SSDFixture, SSDExRead_Normal) {
	SSDExecutor ssde;
	TS_function shell(&ssde);
	bool expected = 0;
	EXPECT_EQ(expected, shell.read(lba));
}
#endif
