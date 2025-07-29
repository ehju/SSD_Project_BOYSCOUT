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
		bool returnval = parser.invalidCommandCheck(cmd);
		EXPECT_EQ(true, returnval);
	}
}
TEST(INVALIDCMD, ISInVALIDCOMMAND)
{

	CommandParser parser;
	bool returnval = parser.invalidCommandCheck("test");
	EXPECT_EQ(false, returnval);
}

TEST(INVALIDCMD,CHECKREADPARAMNUM_PASS)
{	
	CommandParser parser;
	vector<string> cmd = {"read","3" };
	bool returnval = parser.checkParamNum(cmd);
	EXPECT_EQ(true, returnval);	
}
TEST(INVALIDCMD, CHECKREADPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "read","3","0x1234"};
	bool returnval = parser.checkParamNum(cmd);
	EXPECT_EQ(false, returnval);

}