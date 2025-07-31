#pragma once
#include <iostream>
#include <string>
#include <vector>
enum class SSDCommand
{
	SSDCommand_WRITE = 0,
	SSDCommand_READ,
	SSDCommand_ERASE,
	SSDCommand_FLUSH,
	SSDCommand_Count,
	SSDCommand_INVALID,
};
struct CommandInfo
{
	unsigned int command;
	unsigned int lba;
	unsigned int value;
};

struct CommandFormat
{
	std::string cmd;
	int paramnum;
	bool isUseLBA;
	bool isUseValue;
	bool isUseSize;
	unsigned int cmdIndex;
};

class CommandParser
{
public:
	virtual CommandInfo parse(int argc, char* argv[]);
	/*{
		CommandInfo ret = { 0, 0, 0 };
		return ret;
	}*/
	bool isValidCommand(std::vector<std::string> str);
	CommandInfo MakeCommandInfo(std::vector<std::string> str);


private:
	std::vector<CommandFormat> commandlist = {
		{"W",2,true,true,false,(unsigned int)SSDCommand::SSDCommand_WRITE},
		{"R",1,true,false,false,(unsigned int)SSDCommand::SSDCommand_READ},
		{"E",2,true,false,true, (unsigned int)SSDCommand::SSDCommand_ERASE},
	};
	const int CMDINDEX = 0;
	const int LBAINDEX = 1;
	const int VALUEINDEX = 2;
	const int SIZEINDEX = 2;
	const int LBAMAXLENGTH = 2;
	const int SIZEMAXLENGTH = 2;
	const int VALUESTART = 2;
	const int VALUELENGTH = 10;
	const int MAXSIZE = 10;
	bool checkCommand(std::vector<std::string> str);
	bool checkParamNum(std::vector<std::string> str);
	bool checkValidLBA(std::vector<std::string> str);
	bool checkValidValue(std::vector<std::string> str);
	bool checkValidSize(std::vector<std::string> cmdSplits);
	unsigned int getLBA(const CommandFormat& cmddata, const std::vector<std::string>& strlist);
	unsigned int getSize(const CommandFormat& cmddata, const std::vector<std::string>& strlist);
	unsigned int getDecimal(const std::string& str);
	unsigned int getHexValue(const CommandFormat& cmddata, const std::vector<std::string>& strlist);
	
};