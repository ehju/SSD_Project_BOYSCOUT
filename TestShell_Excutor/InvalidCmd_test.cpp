#include <gmock/gmock.h>
#include <iostream>
#include "CommandParser.h"
using std::string;
using namespace testing;

TEST(INVALIDCMD, ISVALIDCOMMAND)
{

	CommandParser parser;
	string command[] = {"write","read","exit","help","fullwrite","fullread"};
	for (string cmd : command)
	{
		bool returnval = parser.InvalidCheck(cmd);
		EXPECT_EQ(true, returnval);
	}

}
TEST(INVALIDCMD,CHECKREADPARAMNUM_PASS)
{
	
	CommandParser parser;
	
	bool returnval = parser.checkParamNum("read 3");
	EXPECT_EQ(true, returnval);
	
}
TEST(INVALIDCMD, CHECKREADPARAMNUM_FAIL)
{

	CommandParser parser;
	bool returnval = parser.checkParamNum("read 3 0x1234");
	EXPECT_EQ(true, returnval);

}