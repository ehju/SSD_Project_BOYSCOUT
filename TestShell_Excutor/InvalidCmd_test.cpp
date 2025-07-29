#include <gmock/gmock.h>
#include <iostream>
#include "CommandParser.h"
using std::string;
using namespace testing;

TEST(INVALIDCMD, ISVALIDCOMMAND)
{
	//지정된 command인지
	CommandParser parser;
	string command[] = { "write","read","exit","help","fullwrite","fullread"};
	for (string cmd : command)
	{
		bool returnval = parser.InvalidCheck(cmd);
		EXPECT_EQ(true, returnval);
	}

}