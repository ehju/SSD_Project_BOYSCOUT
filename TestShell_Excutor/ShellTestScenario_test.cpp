#include "gmock/gmock.h"
#include "ShellTestScenarios.h"
#include <vector>

using namespace testing;
using std::vector;

class MockSSD : public iTS_SSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(bool, write, (int lba, unsigned int data), (override));
	MOCK_METHOD(bool, erase, (int lba, int size), (override));
	MOCK_METHOD(bool, flush, (), (override));
};

class TestScenarioFixture : public Test {
public:
	MockSSD ssd;
	TestScenario shell{ &ssd };
	int lba = 0;
	unsigned int data = 0x12345678;
};
TEST_F(TestScenarioFixture, ReadCompareCallSSDRead) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;
	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(true, shell.readCompare(lba, readData));
}

TEST_F(TestScenarioFixture, ReadCompareDifferentDataFail) {

	unsigned int writtenData = 0x12345678;
	unsigned int readData = 0x87654321;
	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(false, shell.readCompare(lba, writtenData));
}

TEST_F(TestScenarioFixture, DISABLED_FullWriteAndReadCompareShouldCallFullRangeSSDcommand) {
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

TEST_F(TestScenarioFixture, FullWriteAndReadCompare_WriteFail) {
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

TEST_F(TestScenarioFixture, FullWriteAndReadCompare_ReadFail) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = 0x87654321;

	EXPECT_CALL(ssd, write(_, _))
		.WillRepeatedly(Return(true));

	EXPECT_CALL(ssd, read(_))
		.WillOnce(Return(writtenData))
		.WillRepeatedly(Return(readData));
	EXPECT_EQ(false, shell.fullWriteAndReadCompare());
}

TEST_F(TestScenarioFixture, PartialWriteLBA_behaviorTest) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;
	EXPECT_CALL(ssd, write(_, writtenData))
		.Times(150)
		.WillRepeatedly(Return(true));

	EXPECT_CALL(ssd, read(_))
		.Times(150)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(true, shell.partialLBAWrite());
}

TEST_F(TestScenarioFixture, PartialWriteLBA_WriteFail) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;

	EXPECT_CALL(ssd, write(_, _))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	EXPECT_CALL(ssd, read(_))
		.WillRepeatedly(Return(writtenData));
	EXPECT_EQ(false, shell.partialLBAWrite());
}

TEST_F(TestScenarioFixture, PartialWriteLBA_ReadFail) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = 0x87654321;

	EXPECT_CALL(ssd, write(_, _))
		.WillRepeatedly(Return(true));

	EXPECT_CALL(ssd, read(_))
		.WillOnce(Return(writtenData))
		.WillRepeatedly(Return(readData));
	EXPECT_EQ(false, shell.partialLBAWrite());
}
