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
	void setUpFullWriteAndReadCompare(){
		redirectBufferSetup();
		cmdInfo.command = CommandType::CMD_TS_FullWriteAndReadCompare;
	}
	void setUpPartialWriteLBA() {
		redirectBufferSetup();
		cmdInfo.command = CommandType::CMD_TS_PartialLBAWrite;
	}
	void setUpWriteReadAging() {
		redirectBufferSetup();
		cmdInfo.command = CommandType::CMD_TS_WriteReadAging;
	}
	void setUpEraseWriteAging() {
		redirectBufferSetup();
		cmdInfo.command = CommandType::CMD_TS_EraseWriteAging;
	}
	void redirectBufferSetup() {
		old = std::cout.rdbuf(buffer.rdbuf());
	}
	void checkBufferOutput(std::string expected) {
		std::cout.rdbuf(old);
		string output = buffer.str();
		EXPECT_EQ(output, expected);
	}
	MockSSD ssd;
	TestScenario* shell = TestScenario::GetInstance(&ssd);
	//TestScenario shell{ &ssd };
	CommandInfo cmdInfo;
	int lba = 0;
	unsigned int data = 0x12345678;
	std::stringstream buffer;
	std::streambuf* old;
};
TEST_F(TestScenarioFixture, ReadCompareCallSSDRead) {
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;
	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(true, shell->readCompare(lba, readData));
}

TEST_F(TestScenarioFixture, ReadCompareDifferentDataFail) {

	unsigned int writtenData = 0x12345678;
	unsigned int readData = 0x87654321;
	EXPECT_CALL(ssd, read(lba))
		.Times(1)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(false, shell->readCompare(lba, writtenData));
}

TEST_F(TestScenarioFixture, DISABLED_FullWriteAndReadCompareShouldCallFullRangeSSDcommand) {
	setUpFullWriteAndReadCompare();
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;
	EXPECT_CALL(ssd, write(_, writtenData))
		.Times(100)
		.WillRepeatedly(Return(true));

	EXPECT_CALL(ssd, read(_))
		.Times(100)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(true, shell->execute(cmdInfo));
	checkBufferOutput("Pass\n");
}

TEST_F(TestScenarioFixture, FullWriteAndReadCompare_WriteFail) {
	setUpFullWriteAndReadCompare();
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;

	EXPECT_CALL(ssd, write(_, _))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	EXPECT_CALL(ssd, read(_))
		.WillRepeatedly(Return(writtenData));
	EXPECT_EQ(false, shell->execute(cmdInfo));
	checkBufferOutput("FAIL!\n");
}

TEST_F(TestScenarioFixture, FullWriteAndReadCompare_ReadFail) {
	setUpFullWriteAndReadCompare();
	unsigned int writtenData = 0x12345678;
	unsigned int readData = 0x87654321;

	EXPECT_CALL(ssd, write(_, _))
		.WillRepeatedly(Return(true));

	EXPECT_CALL(ssd, read(_))
		.WillOnce(Return(writtenData))
		.WillRepeatedly(Return(readData));
	EXPECT_EQ(false, shell->execute(cmdInfo));
	checkBufferOutput("FAIL!\n");
}

TEST_F(TestScenarioFixture, PartialWriteLBA_behaviorTest) {
	setUpPartialWriteLBA();
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;
	EXPECT_CALL(ssd, write(_, writtenData))
		.Times(150)
		.WillRepeatedly(Return(true));

	EXPECT_CALL(ssd, read(_))
		.Times(150)
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(true, shell->execute(cmdInfo));
	checkBufferOutput("Pass\n");
}

TEST_F(TestScenarioFixture, PartialWriteLBA_WriteFail) {
	setUpPartialWriteLBA();
	unsigned int writtenData = 0x12345678;
	unsigned int readData = writtenData;

	EXPECT_CALL(ssd, write(_, _))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	EXPECT_CALL(ssd, read(_))
		.WillRepeatedly(Return(writtenData));
	EXPECT_EQ(false, shell->execute(cmdInfo));
	checkBufferOutput("FAIL!\n");
}

TEST_F(TestScenarioFixture, PartialWriteLBA_ReadFail) {
	setUpPartialWriteLBA();
	unsigned int writtenData = 0x12345678;
	unsigned int readData = 0x87654321;
	EXPECT_CALL(ssd, write(_, _))
		.WillRepeatedly(Return(true));

	EXPECT_CALL(ssd, read(_))
		.WillOnce(Return(writtenData))
		.WillRepeatedly(Return(readData));

	EXPECT_EQ(false, shell->execute(cmdInfo));

	checkBufferOutput("FAIL!\n");
}

TEST_F(TestScenarioFixture, ReturnsSameInstance) {
	TestScenario* a = TestScenario::GetInstance(&ssd);
	TestScenario* b = TestScenario::GetInstance(&ssd);

	EXPECT_EQ(a, b);  // 悼老茄 林家咯具 窃
}

