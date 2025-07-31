#include "gmock/gmock.h"
#include "ssd.cpp"
using namespace testing;
class CommandParserTS :public Test {
public:
	CommandParser commandParser;
	int argumentNum;
	char* argumentPointer[10];

	void setInput(int num, char* arg1,  char* arg2, char* arg3)
	{
		argumentNum = num;
		char argu0[32] = "SSD.exe";
		argumentPointer[0] = argu0;
		argumentPointer[1] = arg1;
		argumentPointer[2] = arg2;
		argumentPointer[3] = arg3;

	}
	void checkExpected(const CommandInfo& expected, const CommandInfo& actual)
	{
		EXPECT_EQ(expected.command, actual.command);
		EXPECT_EQ(expected.lba, actual.lba);
		EXPECT_EQ(expected.value, actual.value);
	}
	
};
TEST_F(CommandParserTS, InvalidCommandTC1)
{
	char argu1[32]="T";	
	setInput(2, argu1,nullptr, nullptr);
	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFF , 0xFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);
}

TEST_F(CommandParserTS, WriteTC1)
{
	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "0xFF23001D";
	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_WRITE, 0 , 0xFF23001D };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, WriteTC2)
{

	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "0xAe129432";

	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = {(unsigned int) SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, WriteTC3)
{
	

	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "0xAe1294322344";
	setInput(4, argu1, argu2, argu3);


	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, WriteTC4)
{
	
	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "ABFF123423";
	setInput(4, argu1, argu2, argu3);


	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, WriteTC5)
{
	
	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "ABFF123423";
	setInput(4, argu1, argu2, argu3);


	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, WriteTC6)
{

	char argu1[32] = "W";
	char argu2[32] = "101";
	char argu3[32] = "0xFFDA1234";
	setInput(4, argu1, argu2, argu3);


	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, WriteTC7)
{
	char argu1[32] = "W";
	setInput(2, argu1, nullptr, nullptr);
	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, ReadTC1)
{


	char argu1[32] = "R";
	char argu2[32] = "0";
	setInput(3, argu1, argu2, nullptr);


	CommandInfo expected = { 1, 0 , 0xFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);
}

TEST_F(CommandParserTS, ReadTC2)
{
	char argu1[32] = "R";
	char argu2[32] = "0";
	char argu3[32] = "0x1";

	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, ReadTC3)
{
	char argu1[32] = "R";
	setInput(2, argu1, nullptr, nullptr);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, ReadTC4)
{
	char argu1[32] = "R";
	char argu2[32] = "101";

	setInput(3, argu1, argu2, nullptr);
	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, EraseTC1)
{
	char argu1[32] = "E";
	char argu2[32] = "0";
	setInput(3, argu1, argu2, nullptr);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFF , 0xFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);
}

TEST_F(CommandParserTS, EraseTC2)
{
	char argu1[32] = "E";
	char argu2[32] = "0";
	char argu3[32] = "-5";

	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, EraseTC3)
{
	char argu1[32] = "E";
	char argu2[32] = "95";
	char argu3[32] = "6";

	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_ERASE, 95 , 6 };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);
}

TEST_F(CommandParserTS, EraseTC4)
{
	char argu1[32] = "E";
	char argu2[32] = "80";
	char argu3[32] = "11";

	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}

TEST_F(CommandParserTS, EraseTC5)
{
	char argu1[32] = "E";
	char argu2[32] = "80";
	char argu3[32] = "0";

	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_ERASE, 80 , 0 };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, EraseTC6)
{
	char argu1[32] = "E";
	char argu2[32] = "80";
	char argu3[32] = "10";

	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_ERASE, 80 , 10 };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);
}
TEST_F(CommandParserTS, EraseTC7)
{
	char argu1[32] = "E";
	char argu2[32] = "80";
	char argu3[32] = "0x01234567";

	setInput(4, argu1, argu2, argu3);

	CommandInfo expected = { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	checkExpected(expected, actual);

}