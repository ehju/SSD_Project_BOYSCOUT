#pragma once
#include "CommandData.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
using std::string;
using std::vector;

struct CommandFormat
{
	string cmd;
	int paramnum;
	int lbaIndex;
	int valueIndex;
	int sizeIndex;
	int endLbaIndex;
	string usage;
};

class CommandParser {
public:
	vector<CommandFormat> commandlist = {

		{"write",2,1,2,0,0," <LBA> <VALUE> \t: LBA = 0~99 , VALUE = 0x00000000~0xFFFFFFFF(10 Digit)// Write Value @LBA"},
		{"read",1,1,0,0,0," <LBA>          \t: LBA = 0~99 // Read @LBA"},
		{"erase",2,1,0,2,0," <LBA> <SIZE>  \t: LBA = 0~99 , SIZE = (+/-Decimal)// Erase Value @LBA ~@LBA+SIZE"},
		{"erase_range",2,1,0,0,2," <START_LBA> <END_LBA>: LBA = 0~99 // Erase @ STARTLBA~ENDLBA"},
		{"fullwrite",1,0,1,0,0," <VALUE>   \t: VALUE = 0x00000000~0xFFFFFFFF(10 Digit) // Write Value @ALL LBA"},
		{"fullread",0,0,0,0,0,"            \t: No Param //Read Full Range"},
		{"flush",0,0,0,0,0,"               \t: No Param //Flush Buffer"},
		{"1_FullWriteAndReadCompare",0,0,0,0,0," : No Param //Write and Read Compare @ AllRange"},
		{"1_",0,0,0,0,0,"		: No Param//Write and Read Compare @ AllRange"},
		{"2_PartialLBAWrite",0,0,0,0,0," : No Param //(Write 0x12345678 @LBA_0~4 & ReadCompare) * 30 times"},
		{"2_",0,0,0,0,0,"		: No Param//(Write 0x12345678++ @LBA_0~4 & ReadCompare) * 30 times"},
		{"3_WriteReadAging",0,0,0,0,0," : No Param //(Write RandomValue @LBA_9 and @LBA_99) * 200 times"},
		{"3_",0,0,0,0,0,"		: No Param//(Write RandomValue @LBA_9 and @LBA_99) * 200 times"},
		{"4_EraseAndWriteAging",0,0,0,0,0," : No Param //(Write/OverWrite/Erase)* 30 times"},
		{"4_",0,0,0,0,0,"		: No Param////(Write/OverWrite/Erase)* 30 times"},
		{"exit",0,0,0,0,0,"		: No Param//Terminate Shell" },
		{"help",0,0,0,0,0,"		: No Param//Print Command Usage"},

		
	};

	CommandInfo createCommandData(const string cmd);
	unsigned int getCommandType(const string cmd);
	bool isValidCommand(vector<string> str);

	void runCommandHelp(void);
;
private:
	const int CMDINDEX = 0;
	
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
		{"flush", CMD_BASIC_FLUSH },
		{"1_FullWriteAndReadCompare", CMD_TS_FullWriteAndReadCompare },
		{"1_", CMD_TS_FullWriteAndReadCompare },
		{"2_PartialLBAWrite", CMD_TS_PartialLBAWrite },
		{"2_", CMD_TS_PartialLBAWrite },
		{"3_WriteReadAging", CMD_TS_WriteReadAging },
		{"3_", CMD_TS_WriteReadAging },
		{"4_EraseAndWriteAging", CMD_TS_EraseWriteAging },
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
	unsigned int getLBA(int lbaIndex, const std::vector<std::string>& strlist);
	unsigned int getSize(const CommandFormat& cmddata, const std::vector<std::string>& strlist);
	int getSignedDecimal(const string& str);
	unsigned int getDecimal(const string& str);
	unsigned int getHexValue(const CommandFormat& cmddata, const vector<string>& strlist);
	bool isNumber(const string& str);
	bool isHex(const string& str);
};
