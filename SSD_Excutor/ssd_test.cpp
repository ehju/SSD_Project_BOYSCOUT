#include "gmock/gmock.h"
#include "ssd.cpp"
#include "SSDHelper.cpp"
class CommandParserMock : public CommandParser
{
public:
	MOCK_METHOD(CommandInfo, parse, (int argc, char* argv[]), (override));
};

class SSDTS : public testing::Test {
protected:
	void SetUp() override
	{
		ssd = std::make_shared<SSD>(&commandParserMock, &writeCommand, &readCommand);
	}

	void TearDown() override
	{
		file.close();
	}

public:
	testing::NiceMock<CommandParserMock> commandParserMock;
	Write writeCommand;
	Read readCommand;
	std::shared_ptr<SSD> ssd;
	int dummyArgc{ 0 };
	char** dummyArgv{ nullptr };
	std::string nand{ "ssd_nand.txt" };
	std::ifstream file;

	const unsigned int WRITE_CMD = 0;
	const unsigned int READ_CMD = 1;
	const unsigned int INVALID_CMD = 3;
	SSDHelper ssdHelper;

	void checkData(unsigned int expectedLba, unsigned int expectedValue, std::string actual)
	{
		std::string expected = "";
		std::ostringstream ss;
		ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << expectedValue;
		expected = std::to_string(expectedLba) + " " + ss.str();
		EXPECT_EQ(expected, actual);
	}

	std::string directAccessNand(unsigned int lba)
	{
		std::string line = "";
		file.seekg(0);
		for (int i = 0; i <= lba; i++)
		{
			getline(file, line);
		}
		return line;
	}
};

TEST_F(SSDTS, SsdWriteTC1)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillRepeatedly(testing::Return(CommandInfo{ 0, 1, 4 }));

	ssd->run(dummyArgc, dummyArgv);

	std::string actual;

	file.open(nand);
	actual = directAccessNand(1);
	checkData(1, 4, actual);
}

TEST_F(SSDTS, SsdWriteTC2)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 0, 2, 4 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 5, 2 }));

	ssd->run(dummyArgc, dummyArgv);
	ssd->run(dummyArgc, dummyArgv);

	std::string actual;

	file.open(nand);
	actual = directAccessNand(2);
	checkData(2, 4, actual);

	actual = directAccessNand(5);
	checkData(5, 2, actual);
}

TEST_F(SSDTS, SsdWriteTC3)
{
	CommandInfo mockReturned = { 0, 2, 4 };
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ 0, 2, 4 }))
		.WillOnce(testing::Return(CommandInfo{ 0, 5, 2 }));

	ssd->run(dummyArgc, dummyArgv);
	ssd->run(dummyArgc, dummyArgv);

	std::string actual;

	file.open(nand);
	actual = directAccessNand(2);
	checkData(2, 4, actual);

	actual = directAccessNand(5);
	checkData(5, 2, actual);
}

TEST_F(SSDTS, SsdReadAfterWrite)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ WRITE_CMD, 1, (unsigned int)0x1 }))
		.WillOnce(testing::Return(CommandInfo{ WRITE_CMD, 2, (unsigned int)0x2 }))
		.WillOnce(testing::Return(CommandInfo{ READ_CMD, 1, 0xFFFFFFFF }))
		.WillOnce(testing::Return(CommandInfo{ READ_CMD, 2, 0xFFFFFFFF }));

	ssd->run(dummyArgc, dummyArgv);
	ssd->run(dummyArgc, dummyArgv);
	ssd->run(dummyArgc, dummyArgv);
	EXPECT_EQ(ssdHelper.getReadResultFromFile(), "0x00000001");
	ssd->run(dummyArgc, dummyArgv);
	EXPECT_EQ(ssdHelper.getReadResultFromFile(), "0x00000002");
}

TEST_F(SSDTS, SSDInavlidCommandShouldPrintOutputError)
{
	EXPECT_CALL(commandParserMock, parse(testing::_, testing::_))
		.WillOnce(testing::Return(CommandInfo{ INVALID_CMD, 0xFFFFFFFF, 0xFFFFFFFF }));
	ssd->run(dummyArgc, dummyArgv);
	EXPECT_EQ(ssdHelper.getReadResultFromFile(), "ERROR");
}