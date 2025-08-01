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

#define REAL_DEBUG 1
class MockSSD : public iTS_SSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(bool, write, (int lba, unsigned int data), (override));
	MOCK_METHOD(bool, erase, (int lba, int size), (override));
	MOCK_METHOD(bool, flush, (), (override));
};

class ShellCommandTest : public Test {
public:
	void SetupRead(){
		command = new Read(& ssd );
		cmdInfo.lba = lba;
	}
	void SetupWrite() {

		command = new Write(&ssd);
		cmdInfo.lba = lba;
		cmdInfo.value = DUMMY_DATA;
	}
	void SetupFullRead() {
		command = new FullRead(&ssd);
		cmdInfo.lba = lba;
	}
	void SetupFullWrite() {
		command = new FullWrite(&ssd);
		cmdInfo.lba = lba;
		cmdInfo.value = DUMMY_DATA;
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
	ShellCommandItem* command;
	int lba = 0;
	unsigned int data;
	const unsigned int DUMMY_DATA = 0x12345678;
	CommandInfo cmdInfo;
	std::stringstream buffer;
	std::streambuf* old;
};

// SSD Read function should be called
TEST_F(ShellCommandTest, ReadBasic) {
	SetupRead();
	EXPECT_CALL(ssd, read(lba))
		.Times(1);

	bool ret = command->execute(cmdInfo);
	EXPECT_EQ(true, ret);
}

// Read Test state test
TEST_F(ShellCommandTest,ReadMockResultTest) {
	SetupRead();
	cmdInfo.lba = 10;
	redirectBufferSetup();

	EXPECT_CALL(ssd, read(cmdInfo.lba))
		.Times(1)
		.WillOnce(testing::Return(DUMMY_DATA));

	bool ret = command->execute(cmdInfo);
	
	checkBufferOutput("[Read] LBA 10 : 0x12345678\n");
}
TEST_F(ShellCommandTest, WriteBasic) {
	SetupWrite();
	redirectBufferSetup();
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
}
TEST_F(ShellCommandTest, ReadFullReadTest) {
	SetupFullRead();
	bool ret;
	EXPECT_CALL(ssd, read(_))
		.Times(100);

	EXPECT_EQ(true, command->execute(cmdInfo));
}

TEST_F(ShellCommandTest, ReadFullReadTestExpectedReturn) {
	SetupFullRead();
	data = 0; // start from 0
	for (int lba = 0; lba < 100; lba++) {
		EXPECT_CALL(ssd, read(lba))
			.Times(1)
			.WillRepeatedly(Return(data));
		data++;
	}
	//expected Result Making 

	EXPECT_EQ(true, command->execute(cmdInfo));
}

TEST_F(ShellCommandTest, FullWriteNormal) {
	SetupFullWrite();
	redirectBufferSetup();
	cmdInfo.value = 0xABCDFFFF;
	EXPECT_CALL(ssd, write(_, cmdInfo.value))
		.Times(100)
		.WillRepeatedly(Return(true));

	EXPECT_EQ(true, command->execute(cmdInfo));
	checkBufferOutput("[Write] Done\n");
}

TEST_F(ShellCommandTest, FullWriteFail) {
	SetupFullWrite();
	cmdInfo.value = 0xABCDFFFF;
	EXPECT_CALL(ssd, write(_, cmdInfo.value))
		.Times(5)
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillOnce(Return(true))
		.WillRepeatedly(Return(false));

	EXPECT_EQ(false, command->execute(cmdInfo));

}



/*
#if REAL_DEBUG
TEST(SSDExecutorInterface, SSDExWrite_Normal) {
	SSDExecutor ssde;

	ShellCommand shell(&ssde);
	bool expected = true;

	EXPECT_EQ(expected, command->execute(cmdInfo));
}

TEST(SSDExecutorInterface, SSDExRead_Normal) {
	SSDExecutor ssde;
	ShellCommand shell(&ssde);
	bool expected = 0;
	EXPECT_EQ(expected, command->execute(cmdInfo));
}
#endif
*/