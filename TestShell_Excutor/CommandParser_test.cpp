#include "gmock/gmock.h"
#include "CommandParser.h"
using namespace testing;

class CommandParserTS :public Test {
public:
	CommandParser commandParser;

	
	void checkExpected(const CommandInfo& expected, const CommandInfo& actual)
	{
		EXPECT_EQ(expected.command, actual.command);
		EXPECT_EQ(expected.lba, actual.lba);
		EXPECT_EQ(expected.value, actual.value);
		EXPECT_EQ(expected.size, actual.size);
	}

};
TEST_F(CommandParserTS, Invalid)
{
	const string command = "invlaid 3 0xAAAABBBB";

	CommandInfo expected = { (unsigned int)CommandType::CMD_NOT_SUPPORTED, 0xFFFFFFFF , 0xFFFFFFFF ,-1};
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, WrteTC1)
{
	const string command = "write 3 0xAAAABBBB";
	CommandInfo expected = { (unsigned int)CommandType::CMD_BASIC_WRITE, 3 , 0xAAAABBBB,-1 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, WrteTC2)
{
	const string command = "write 3";
	CommandInfo expected = { (unsigned int)CommandType::CMD_NOT_SUPPORTED, 0xFFFFFFFF , 0xFFFFFFFF ,-1};
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, WrteTC3)
{
	const string command = "write 3 0";
	CommandInfo expected = { (unsigned int)CommandType::CMD_NOT_SUPPORTED, 0xFFFFFFFF , 0xFFFFFFFF,-1 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, WrteTC4)
{
	const string command = "write 3 0xABCDEFGH";
	CommandInfo expected = { (unsigned int)CommandType::CMD_NOT_SUPPORTED, 0xFFFFFFFF , 0xFFFFFFFF,-1 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, Read)
{
	const string command = "read 3 ";

	CommandInfo expected = { (unsigned int)CommandType::CMD_BASIC_READ, 3 , 0xFFFFFFFF,-1 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, Read1)
{
	const string command = "read 3 0xAAAABBBB";

	CommandInfo expected = { (unsigned int)CommandType::CMD_NOT_SUPPORTED, 0xFFFFFFFF , 0xFFFFFFFF,-1 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, Erase)
{
	const string command = "erase 50 -10";

	CommandInfo expected = { (unsigned int)CommandType::CMD_BASIC_ERASE, 50 , 0xFFFFFFFF,-10 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}

TEST_F(CommandParserTS, EraseRange)
{
	const string command = "erase_range 90 100";

	CommandInfo expected = { (unsigned int)CommandType::CMD_BASIC_ERASE_RANGE, 90 , 100,-1 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}

TEST_F(CommandParserTS, TS1)
{
	const string command = "1_";

	CommandInfo expected = { (unsigned int)CommandType::CMD_TS_FullWriteAndReadCompare, 0xFFFFFFFF , 0xFFFFFFFF,-1 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, TS4)
{
	const string command = "4_";

	CommandInfo expected = { (unsigned int)CommandType::CMD_TS_EraseWriteAging, 0xFFFFFFFF , 0xFFFFFFFF,-1 };
	CommandInfo actual = commandParser.createCommandData(command);
	checkExpected(expected, actual);
}