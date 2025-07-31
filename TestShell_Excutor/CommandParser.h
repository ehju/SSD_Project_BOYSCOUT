#pragma once
#include "gmock/gmock.h"
#include "ShellCommand.h"
#include "CommandData.h"
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

struct CommandFormat
{
	string cmd;
	int paramnum;
	bool isUseLBA;
	bool isUseValue;
	bool isUseSize;
	bool isUseEndLBA;
	string usage;
};

class CommandParser {
public:
	vector<CommandFormat> commandlist = {
		{"write",2,true,true,false,false," <LBA> <VALUE> : LBA = 0~99 , VALUE = 0x00000000~0xFFFFFFFF(10 Digit)// Write Value @LBA"},
		{"read",1,true,false,false,false," <LBA>         : LBA = 0~99 // Read @LBA"},
		{"erase",2,true,false,true,false," <LBA> <SIZE> : LBA = 0~99 , SIZE = (+/-Decimal)// Erase Value @LBA ~@LBA+SIZE"},
		{"erase_range",2,true,false,false,true," <START_LBA> <END_LBA>: LBA = 0~99 // Erase @ STARTLBA~ENDLBA"},
		{"fullwrite",1,false,true,false,false," <VALUE>  : VALUE = 0x00000000~0xFFFFFFFF(10 Digit) // Write Value @ALL LBA"},
		{"fullread",0,false,false,false,false,"          : No Param //Read Full Range"},
		{"1_FullWriteAndReadCompare",0,false,false,false,false," : No Param //Write and Read Compare @ AllRange"},
		{"1_",0,false,false,false,false,"		: No Param//Write and Read Compare @ AllRange"},
		{"2_PartialLBAWrite",0,false,false,false,false," : No Param //(Write 0x12345678 @LBA_0~4 & ReadCompare) * 30 times"},
		{"2_",0,false,false,false,false,"		: No Param//(Write 0x12345678++ @LBA_0~4 & ReadCompare) * 30 times"},
		{"3_WriteReadAging",0,false,false,false,false," : No Param //(Write RandomValue @LBA_9 and @LBA_99) * 200 times"},
		{"3_",0,false,false,false,false,"		: No Param//(Write RandomValue @LBA_9 and @LBA_99) * 200 times"},
		{"4_EraseAndWriteAging",0,false,false,false,false," : No Param //(Write/OverWrite/Erase)* 30 times"},
		{"4_",0,false,false,false,false,"		: No Param////(Write/OverWrite/Erase)* 30 times"},
		{"exit",0,false,false,false,false,"		: No Param//Terminate Shell" },
		{"help",0,false,false,false,false,"		: No Param//Print Command Usage"},
	};
	

	CommandInfo createCommandData(const string cmd);
	unsigned int getCommandType(const string cmd);
	bool isValidCommand(vector<string> str);

	int runCommand(const string cmd);
	int runSubCommands(vector<string> cmdParms, int type);//CommandInfo cmddata
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
	const int ENDLBAINDEX = 2;
	const int VALUEINDEX = 2;
	const int SIZEINDEX = 2;
	const int LBAMAXLENGTH = 2;
	const int HEXSTART = 2;
	const int VALUELENGTH = 10;
	const int SIZEMAXLENGTH = 2;
	const string teamName = "BOYSCOUT";
	const string teamLeader = "¹Ú¼¼¿î";
	const string teamMemberName = "ÀÌ½ÂÇö/ÁÖÀºÇý/Á¤Áø¼·/ÇãÈÆ/Á¤ÇýÁø";
	
	std::unordered_map<string, int> cmdMap = {
		{"write", CMD_BASIC_WRITE },
		{"read", CMD_BASIC_READ },
		{"erase", CMD_BASIC_ERASE },
		{"erase_range", CMD_BASIC_ERASE_RANGE },
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
		{"4_WriteReadAging", CMD_TS_EraseWriteAging },
		{"4_", CMD_TS_EraseWriteAging },
	};
	vector<string> getCommandParams(const std::string& cmd);
	
	bool checkCommand(vector<string> str);
	bool checkParamNum(vector<string> str);
	bool checkValidLBA(vector<string> str);
	bool checkValidValue(vector<string> str);
	bool checkValidSize(vector<string> cmdSplits);
	bool checkValiEndLBA(vector<string> cmdSplits);

	CommandInfo MakeInvalidCmdData();
	unsigned int getLBA(const CommandFormat& cmddata, const std::vector<std::string>& strlist);
	unsigned int getEndLBA(const CommandFormat& cmddata, const std::vector<std::string>& strlist);

	unsigned int getSize(const CommandFormat& cmddata, const std::vector<std::string>& strlist);
	int getSignedDecimal(const string& str);
	unsigned int getDecimal(const string& str);
	unsigned int getHexValue(const CommandFormat& cmddata, const vector<string>& strlist);
	bool isNumber(const string& str);
	bool isHex(const string& str);
#ifdef _DEBUG
	ShellCommand shell{ new testing::NiceMock<MockSSD>()};
#else
	ShellCommand shell{ new SSDExecutor()};
#endif
};
