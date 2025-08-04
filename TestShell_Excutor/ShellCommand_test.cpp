#include "gmock/gmock.h"
#include "ShellCommand.h"
#include "ShellRead.h"
#include "ShellWrite.h"
#include "ShellFullRead.h"
#include "ShellFullWrite.h"
#include "ShellFlush.h"
#include "ShellErase.h"
#include "ShellEraseRange.h"
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

class ShellCommandTest : public Test {
public:
	void SetUp() override{
		old = std::cout.rdbuf(buffer.rdbuf());
	}
	void TearDown() override {
		std::cout.rdbuf(old);
	}
	void SetupRead(){
		command = std::make_shared<Read>(&ssd);
		cmdInfo.lba = DUMMY_LBA;
	}
	void SetupWrite() {
		command = std::make_shared<Write>(&ssd);
		cmdInfo.lba = DUMMY_LBA;
		cmdInfo.value = DUMMY_DATA;
	}
	void SetupFullRead() {
		command = std::make_shared<FullRead>(&ssd);
		cmdInfo.lba = DUMMY_LBA;
	}
	void SetupFullWrite() {
		command = std::make_shared<FullWrite>(&ssd);
		cmdInfo.lba = DUMMY_LBA;
		cmdInfo.value = DUMMY_DATA;
	}
	void SetupErase() {
		command = std::make_shared<Erase>(&ssd);
		cmdInfo.lba = DUMMY_LBA;
		cmdInfo.value = DUMMY_DATA;
	}
	void SetupEraseRange() {
		command = std::make_shared<EraseRange>(&ssd);
		cmdInfo.lba = DUMMY_LBA;
		cmdInfo.value = DUMMY_DATA;
	}
	void checkBufferOutput(std::string expected) {
		string output = buffer.str();
		EXPECT_EQ(output, expected);
	}
	std::string generateExpectedFullReadLog(unsigned int startData) {
		std::ostringstream oss;
		for (int i = 0; i < 100; ++i) {
			oss << "[Read] LBA " << std::to_string(i) << " : 0x"
				<< std::setw(8) << std::setfill('0') << std::hex << std::uppercase << startData+i << "\n";
		}
		return oss.str();
	}
	NiceMock<MockSSD> ssd;
	std::shared_ptr<ShellCommandItem> command;
	const int DUMMY_LBA = 0;
	const unsigned int DUMMY_DATA = 0x12345678;
	const unsigned int DIFFERENT_DATA = 0xABCDFFFF;
	CommandInfo cmdInfo;
	std::stringstream buffer;
	std::streambuf* old;
};

// SSD Read function should be called
TEST_F(ShellCommandTest, ReadBehaviorTest) {
	SetupRead();
	EXPECT_CALL(ssd, read(cmdInfo.lba))
		.Times(1);

	bool ret = command->execute(cmdInfo);
	EXPECT_EQ(true, ret);
}

// Read Test state test
TEST_F(ShellCommandTest,ReadDummyDataTest) {
	SetupRead();
	cmdInfo.lba = 10;

	EXPECT_CALL(ssd, read(cmdInfo.lba))
		.Times(1)
		.WillOnce(testing::Return(DUMMY_DATA));

	bool ret = command->execute(cmdInfo);
	
	checkBufferOutput("[Read] LBA 10 : 0x12345678\n");
}
TEST_F(ShellCommandTest, WriteBasic) {
	SetupWrite();
	EXPECT_CALL(ssd, write(cmdInfo.lba, cmdInfo.value))
		.Times(1)
		.WillOnce(Return(true));

	EXPECT_EQ(true, command->execute(cmdInfo));
	checkBufferOutput("[Write] Done\n");
}

TEST_F(ShellCommandTest, NoWriteOutOfRangeLBA1) {
	SetupWrite();
	cmdInfo.lba = 100;

	EXPECT_CALL(ssd, write(cmdInfo.lba, cmdInfo.value))
		.Times(0);

	EXPECT_EQ(false, command->execute(cmdInfo));
}

TEST_F(ShellCommandTest, NoWriteOutOfRangeLBA2) {
	SetupWrite();
	cmdInfo.lba = -2;


	EXPECT_CALL(ssd, write(cmdInfo.lba, cmdInfo.value))
		.Times(0);
	EXPECT_EQ(false, command->execute(cmdInfo));
	TearDown();
}
TEST_F(ShellCommandTest, FullReadTest) {
	SetupFullRead();
	EXPECT_CALL(ssd, read(_))
		.Times(100);

	EXPECT_EQ(true, command->execute(cmdInfo));
}

TEST_F(ShellCommandTest, FullReadTestExpectedReturn) {
	SetupFullRead();
	unsigned int data = 0;
	{
		InSequence seq;
		for (int lba = 0; lba < 100; lba++) {
			EXPECT_CALL(ssd, read(lba))
				.Times(1)
				.WillRepeatedly(Return(data));
			data++;
		}
	}

	EXPECT_EQ(true, command->execute(cmdInfo));
	checkBufferOutput(generateExpectedFullReadLog(0));
}

TEST_F(ShellCommandTest, FullWriteNormal) {
	SetupFullWrite();
	cmdInfo.value = DIFFERENT_DATA;
	EXPECT_CALL(ssd, write(_, cmdInfo.value))
		.Times(100)
		.WillRepeatedly(Return(true));

	EXPECT_EQ(true, command->execute(cmdInfo));
	checkBufferOutput("[Write] Done\n");
}

TEST_F(ShellCommandTest, FullWriteFail) {
	SetupFullWrite();
	cmdInfo.value = DIFFERENT_DATA;
	EXPECT_CALL(ssd, write(_, cmdInfo.value))
		.Times(5)
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	EXPECT_EQ(false, command->execute(cmdInfo));
	checkBufferOutput("");
}

TEST_F(ShellCommandTest, ERASE_RANGE_BASIC_TEST) {
	SetupEraseRange();
	cmdInfo.lba = 1;
	cmdInfo.value = 1;
	EXPECT_CALL(ssd, erase(_, _))
		.Times(1)
		.WillRepeatedly(Return(true));

	EXPECT_EQ(true, command->execute(cmdInfo));
}

TEST_F(ShellCommandTest, ERASE_RANGE_21_TEST) {
	SetupEraseRange();
	cmdInfo.lba = 79;
	cmdInfo.value = 99;
	EXPECT_CALL(ssd, erase(_, _))
		.Times(3)
		.WillRepeatedly(Return(true));

	EXPECT_EQ(true, command->execute(cmdInfo));
}

TEST_F(ShellCommandTest, ERASE_RANGE_OPPOSITE_TEST) {
	SetupEraseRange();
	cmdInfo.lba = 99;
	cmdInfo.value = 79;
	EXPECT_CALL(ssd, erase(_, _))
		.Times(3)
		.WillRepeatedly(Return(true));

	EXPECT_EQ(true, command->execute(cmdInfo));
}