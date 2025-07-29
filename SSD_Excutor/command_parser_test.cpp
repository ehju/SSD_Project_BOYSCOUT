#include "gmock/gmock.h"
#include "ssd.cpp"

TEST(CommandParserTS, TC1)
{
	SSD ssd;

	int argumentNum = 3;
	char* argumentPointer[10];

	char argu1[32] = "W";
	char argu2[32] = "0";
	char argu3[32] = "0x1";


	argumentPointer[0] = argu1;
	argumentPointer[1] = argu2;
	argumentPointer[2] = argu3;


	CommandInfo actual = ssd.commandParser.parse(argumentNum, argumentPointer);
}