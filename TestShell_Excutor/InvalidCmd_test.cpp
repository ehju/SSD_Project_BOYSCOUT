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
		bool returnval = parser.isValidCommand(cmd);
		EXPECT_EQ(true, returnval);
	}
}
TEST(INVALIDCMD, ISInVALIDCOMMAND)
{

	CommandParser parser;
	vector<string > cmdList = { "test","93","0x1234" };
	bool returnval = parser.isValidCommand(cmdList);
	EXPECT_EQ(false, returnval);
}

TEST(INVALIDCMD,CHECKREADPARAMNUM_PASS)
{	
	CommandParser parser;
	vector<string> cmd = {"read","3" };
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(true, returnval);	
}
TEST(INVALIDCMD, CHECKREADPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "read","3","0x1234"};
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(false, returnval);
}
TEST(INVALIDCMD, CHECKWRITEPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "write","3","0x12345678" };
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKWRITEPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "write","3"};
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(false, returnval);

}
TEST(INVALIDCMD, CHECKFULLWRITEPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "fullwrite","0x12345678"};
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKFULLWRITEPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "fullwrite","3","0x12345678" };
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(false, returnval);

}

TEST(INVALIDCMD, CHECKFULLREADPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "fullread"};
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKFULLREADPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "fullread","3","0x12345678" };
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(false, returnval);

}

TEST(INVALIDCMD, CHECKEXITPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "exit" };
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKEXITPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "exit","3","0x12345678" };
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(false, returnval);

}
TEST(INVALIDCMD, CHECKHELPPARAMNUM_PASS)
{
	CommandParser parser;
	vector<string> cmd = { "help" };
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(true, returnval);

}
TEST(INVALIDCMD, CHECKHELPARAMNUM_FAIL)
{
	CommandParser parser;
	vector<string> cmd = { "help","3","0x12345678" };
	bool returnval = parser.isValidCommand(cmd);
	EXPECT_EQ(false, returnval);

}
TEST(INVALIDCMD, CHECKLBA_NOTNUMBER)
{
	CommandParser parser;
	vector<string> wrcmd = { "write","ee","0x12345678" };
	
	bool returnval = parser.isValidCommand(wrcmd);
	EXPECT_EQ(false, returnval);
}
TEST(INVALIDCMD, CHECKLBA_FAIL)
{
	CommandParser parser;
	vector<string> wrcmd = { "write","200","0x12345678" };
	bool returnval = parser.isValidCommand(wrcmd);
	vector<string> rdcmd = { "read","200" };
	returnval = parser.isValidCommand(rdcmd);
	EXPECT_EQ(false, returnval);
}
TEST(INVALIDCMD, CHECKLBA_PASS)
{
	CommandParser parser;
	vector<string> wrcmd = { "write","93","0x12345678" };
	bool returnval = parser.isValidCommand(wrcmd);
	vector<string> rdcmd = { "read","93" };
	returnval = parser.isValidCommand(rdcmd);
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
		if (parser.isValidCommand(cmd) == true)
			break;
		passIndex++;
	}
	
	EXPECT_EQ(10, passIndex);
}
TEST(INVALIDCMD, ERASESIZEFAIL)
{
	CommandParser parser;
	vector<vector<string>> invalidcmdlist = {
		{ "erase","93","0x12345678AAA" }, //Decimal
		{ "erase","93","0xABCD" }, //Decimal
		{ "erase","93","STEERW" }, //Decimal
		{ "erase","-10","50" }, //Decimal

	};
	int passIndex = 0;
	for (vector<string> cmd : invalidcmdlist)
	{
		if (parser.isValidCommand(cmd) == true)
			break;
		passIndex++;
	}

	EXPECT_EQ(4, passIndex);
}

TEST(INVALIDCMD, ERASEPASS)
{
	CommandParser parser;
	vector<vector<string>> validcmdlist = {
		{ "erase","93","10" },
		{ "erase","93","-10" },
		{ "erase","93","0" },
		{ "erase","93","0" },
		
	};
	int passIndex = 0;
	for (vector<string> cmd : validcmdlist)
	{
		if (parser.isValidCommand(cmd) == false)
			break;
		passIndex++;
	}

	EXPECT_EQ(4, passIndex);
}
TEST(INVALIDCMD, ERASERANGEFAIL)
{
	CommandParser parser;
	vector<vector<string>> invalidcmdlist = {		
		{ "erase_range","95","-5" },//FAIL	
		{ "erase_range","-5","10" },//FAIL	
		{ "erase_range","0","100" },//pass

	};
	int passIndex = 0;
	for (vector<string> cmd : invalidcmdlist)
	{
		if (parser.isValidCommand(cmd) == true)
			break;
		passIndex++;
	}

	EXPECT_EQ(3, passIndex);
}

TEST(INVALIDCMD, ERASERANGEPASS)
{
	CommandParser parser;
	vector<vector<string>> validcmdlist = {
		{ "erase_range","95","5" },//PASS	
		{ "erase_range","93","97" },//pass
		{ "erase_range","0","97" },//pass

	};
	int passIndex = 0;
	for (vector<string> cmd : validcmdlist)
	{
		if (parser.isValidCommand(cmd) == false)
			break;
		passIndex++;
	}

	EXPECT_EQ(3, passIndex);
}