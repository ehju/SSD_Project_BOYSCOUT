#pragma once
#include "gmock/gmock.h"
#include "ShellCommand.h"

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
using std::string;
using std::vector;

class MockSSD : public iTS_SSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(bool, write, (int lba, unsigned int data), (override));
	MOCK_METHOD(bool, erase, (int lba, int size), (override));
	MOCK_METHOD(bool, flush, (), (override));
};

enum CommandType {
	CMD_NOT_SUPPORTED = -1,
	CMD_BASIC_WRITE = 0,
	CMD_BASIC_READ,
	CMD_BASIC_EXIT,
	CMD_BASIC_HELP,
	CMD_BASIC_FULLWRITE,
	CMD_BASIC_FULLREAD,
	CMD_TS_FullWriteAndReadCompare,
	CMD_TS_PartialLBAWrite,
	CMD_TS_WriteReadAging,
	CMD_MAX
};

struct CommandFormat
{
	string cmd;
	int paramnum;
	bool isUseLBA;
	bool isUseValue;
	string usage;
};

class CommandParser {
public:
	vector<CommandFormat> commandlist = {
		{"write",2,true,true," <LBA> <VALUE> : LBA = 0~99 , VALUE = 0x00000000~0xFFFFFFFF(10 Digit)// Write Value @LBA"},
		{"read",1,true,false," <LBA>         : LBA = 0~99 // Read @LBA"},	
		{"fullwrite",1,false,true," <VALUE>  : VALUE = 0x00000000~0xFFFFFFFF(10 Digit) // Write Value @ALL LBA"},
		{"fullread",0,false,false,"          : No Param //Read Full Range"},
		{"1_FullWriteAndReadCompare",0,false,false," : No Param //Write and Read Compare @ AllRange"},
		{"1_",0,false,false,"		: No Param//Write and Read Compare @ AllRange"},
		{"2_PartialLBAWrite",0,false,false," : No Param //(Write 0x12345678 @LBA_0~4 & ReadCompare) * 30 times"},
		{"2_",0,false,false,"		: No Param//(Write 0x12345678 @LBA_0~4 & ReadCompare) * 30 times"},
		{"3_WriteReadAging",0,false,false," : No Param //(Write RandomValue @LBA_9 and @LBA_99) * 200 times"},
		{"3_",0,false,false,"		: No Param//(Write RandomValue @LBA_9 and @LBA_99) * 200 times"},
		{"exit",0,false,false,"		: No Param//Terminate Shell" },
		{"help",0,false,false,"		: No Param//Print Command Usage"},
	};
	int runCommand(const string cmd);
	vector<string> getCommandParams(const std::string& cmd);
	int getCommandType(const string cmd);
	bool isValidCommand(vector<string> str);
	
	int runSubCommands(vector<string> cmdParms, int type);
	bool runCommandWrite(const string lba, const string value);
	int runCommandRead(const string lba);
	void printReadResult(int lba, unsigned int value);
	void runCommandHelp(void);
	bool runCommandFullWrite(const string value);
	int runCommandFullRead(void);
	int runCommandTestScenario(int type);
private:
	const int CMDINDEX = 0;
	const int LBAINDEX =1;
	const int VALUEINDEX = 2;
	const int LBAMAXLENGTH = 2;
	const int VALUESTART = 2;
	const int VALUELENGTH = 10;
	const string teamName = "BOYSCOUT";
	const string teamLeader = "¹Ú¼¼¿î";
	const string teamMemberName = "ÀÌ½ÂÇö/ÁÖÀºÇý/Á¤Áø¼·/ÇãÈÆ/Á¤ÇýÁø";
	
	std::unordered_map<string, int> cmdMap = {
		{"write", CMD_BASIC_WRITE },
		{"read", CMD_BASIC_READ },
		{"exit", CMD_BASIC_EXIT },
		{"help", CMD_BASIC_HELP },
		{"fullwrite", CMD_BASIC_FULLWRITE },
		{"fullread", CMD_BASIC_FULLREAD },
		{"1_FullWriteAndReadCompare", CMD_TS_FullWriteAndReadCompare },
		{"1_", CMD_TS_FullWriteAndReadCompare },
		{"2_PartialLBAWrite", CMD_TS_PartialLBAWrite },
		{"2_", CMD_TS_PartialLBAWrite },
		{"3_WriteReadAging", CMD_TS_WriteReadAging },
		{"3_", CMD_TS_WriteReadAging },
	};

	bool checkCommand(vector<string> str);
	bool checkParamNum(vector<string> str);
	bool checkValidLBA(vector<string> str);
	bool checkValidValue(vector<string> str);
#ifdef _DEBUG
	ShellCommand shell{ new testing::NiceMock<MockSSD>()};
#else
	ShellCommand shell{ new SSDExecutor()};
#endif
};
