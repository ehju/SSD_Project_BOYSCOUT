#include "gmock/gmock.h"
#include "ssd.cpp"

TEST(CommandParserTS, InvalidCommandTC1)
{
	CommandParser commandParser;

	int argumentNum = 2;
	char* argumentPointer[10];

	char argu1[32] = "T";

	argumentPointer[0] = argu1;

	CommandInfo expected = { 0xFFFFFFFF, 0xFFFFFFFF , 0xFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);

}

TEST(CommandParserTS, WriteTC1)
{
	CommandParser commandParser;

	int argumentNum = 4;
	char* argumentPointer[10];

	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "0xFF23001D";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;
	argumentPointer[2] = argu3;


	CommandInfo expected = { 0, 0 , 0xFF23001D };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, WriteTC2)
{
	CommandParser commandParser;

	int argumentNum = 4;
	char* argumentPointer[10];

	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "0xAe129432";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;
	argumentPointer[2] = argu3;


	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, WriteTC3)
{
	CommandParser commandParser;

	int argumentNum = 4;
	char* argumentPointer[10];

	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "0xAe1294322344";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;
	argumentPointer[2] = argu3;


	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, WriteTC4)
{
	CommandParser commandParser;

	int argumentNum = 4;
	char* argumentPointer[10];

	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "ABFF123423";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;
	argumentPointer[2] = argu3;


	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, WriteTC5)
{
	CommandParser commandParser;

	int argumentNum = 4;
	char* argumentPointer[10];

	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "ABFF123423";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;
	argumentPointer[2] = argu3;


	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, WriteTC6)
{
	CommandParser commandParser;

	int argumentNum = 4;
	char* argumentPointer[10];

	char argu1[32] = "W";
	char argu2[32] = "101";
	char argu3[32] = "0xFFDA1234";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;
	argumentPointer[2] = argu3;


	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, WriteTC7)
{
	CommandParser commandParser;

	int argumentNum = 2;
	char* argumentPointer[10];

	char argu1[32] = "W";

	argumentPointer[0] = argu1;

	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, ReadTC1)
{
	CommandParser commandParser;

	int argumentNum = 3;
	char* argumentPointer[10];

	char argu1[32] = "R";
	char argu2[32] = "0";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;


	CommandInfo expected = { 1, 0 , 0xFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, ReadTC2)
{
	CommandParser commandParser;

	int argumentNum = 4;
	char* argumentPointer[10];

	char argu1[32] = "R";
	char argu2[32] = "0";
	char argu3[32] = "0x1";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;
	argumentPointer[2] = argu3;

	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, ReadTC3)
{
	CommandParser commandParser;

	int argumentNum = 2;
	char* argumentPointer[10];

	char argu1[32] = "R";

	argumentPointer[0] = argu1;

	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}

TEST(CommandParserTS, ReadTC4)
{
	CommandParser commandParser;

	int argumentNum = 3;
	char* argumentPointer[10];

	char argu1[32] = "R";
	char argu2[32] = "101";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;


	CommandInfo expected = { 0xFFFFFFFFF, 0xFFFFFFFFF , 0xFFFFFFFFF };
	CommandInfo actual = commandParser.parse(argumentNum, argumentPointer);
	EXPECT_EQ(expected.command, actual.command);
	EXPECT_EQ(expected.lba, actual.lba);
	EXPECT_EQ(expected.value, actual.value);
}