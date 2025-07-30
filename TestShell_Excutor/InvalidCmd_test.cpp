#include <gmock/gmock.h>
#include <iostream>
#include "CommandParser.h"
using std::string;
using namespace testing;

TEST(INVALIDCMD, ISVALIDCOMMAND)
{

	CommandParser parser;
	vector<vector<string>> cmdList = {
		{ "write","93","0x12345678" },
		{ "read","93" },
		{ "fullwrite","0x12345678" },
		{ "fullread" },
		{ "exit" },
		{ "help"},	
	};

	for (vector<string> cmd : cmdList)
	{
		bool returnval = parser.isInvalidCommand(cmd);
		EXPECT_EQ(true, returnval);
	}
}
TEST(INVALIDCMD, ISInVALIDCOMMAND)
{

	CommandParser parser;
	vector<string > cmdList = { "test","93","0x1234" };
	bool returnval = parser.isInvalidCommand(cmdList);
	EXPECT_EQ(false, returnval);
}

TEST(INVALIDCMD,CHECKREADPARAMNUM_PASS)
{	
	CommandParser parser;
	vector<string> cmd = {"read","3" };
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(true, returnval);	
}
TEST(INVALIDCMD, CHECKREADPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "read","3","0x1234"};
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(false, returnval);
}
TEST(INVALIDCMD, CHECKWRITEPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "write","3","0x12345678" };
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKWRITEPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "write","3"};
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(false, returnval);

}
TEST(INVALIDCMD, CHECKFULLWRITEPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "fullwrite","0x12345678"};
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKFULLWRITEPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "fullwrite","3","0x12345678" };
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(false, returnval);

}

TEST(INVALIDCMD, CHECKFULLREADPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "fullread"};
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKFULLREADPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "fullread","3","0x12345678" };
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(false, returnval);

}

TEST(INVALIDCMD, CHECKEXITPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "exit" };
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKEXITPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "exit","3","0x12345678" };
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(false, returnval);

}
TEST(INVALIDCMD, CHECKHELPPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "help" };
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKHELPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "help","3","0x12345678" };
	bool returnval = parser.isInvalidCommand(cmd);
	EXPECT_EQ(false, returnval);

}
TEST(INVALIDCMD, CHECKLBA_NOTNUMBER)
{
	CommandParser parser;
	vector<string> wrcmd = { "write","ee","0x12345678" };
	
	bool returnval = parser.isInvalidCommand(wrcmd);
	EXPECT_EQ(false, returnval);
}
TEST(INVALIDCMD, CHECKLBA_FAIL)
{
	CommandParser parser;
	vector<string> wrcmd = { "write","200","0x12345678" };
	bool returnval = parser.isInvalidCommand(wrcmd);
	vector<string> rdcmd = { "read","200" };
	returnval = parser.isInvalidCommand(rdcmd);
	EXPECT_EQ(false, returnval);
}
TEST(INVALIDCMD, CHECKLBA_PASS)
{
	CommandParser parser;
	vector<string> wrcmd = { "write","93","0x12345678" };
	bool returnval = parser.isInvalidCommand(wrcmd);
	vector<string> rdcmd = { "read","93" };
	returnval = parser.isInvalidCommand(rdcmd);
	EXPECT_EQ(true, returnval);
}

TEST(INVALIDCMD, CHECKVALUE_FAIL)
{
	CommandParser parser;
	vector<vector<string>> invalidcmdlist = {
		{ "write","93","0x1234" },
		{ "write","93","0x12345678AAA" },
		{ "write","93","qwertyuiop" },
		{ "write","93","01234567x0" },
		{ "write","93","0x1234abcd" },		
		{ "fullwrite","0x1234" },
		{ "fullwrite","0x12345678AAA" },
		{ "fullwrite","qwertyuiop" },
		{ "fullwrite","01234567x0" },
		{ "fullwrite","0x1234abcd" },	

	};
	int passIndex = 0;
	for (vector<string> cmd : invalidcmdlist)
	{
		if (parser.isInvalidCommand(cmd) == true)
			break;
		passIndex++;
	}
	
	EXPECT_EQ(10, passIndex);
}
