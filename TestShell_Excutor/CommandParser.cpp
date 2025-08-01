#include "CommandParser.h"
#include <sstream>
using namespace std;

vector<string> CommandParser::getCommandParams(const string& cmd)
{
	stringstream stream(cmd);
	string word;
	vector<string> tokens;

	while (stream >> word) {
		tokens.push_back(word);
	}
	return tokens;
}

CommandInfo CommandParser::createCommandData(const string cmd)
{
	vector<string> cmdParms = getCommandParams(cmd);
	if (isValidCommand(cmdParms) == false)
		return MakeInvalidCmdData();

	CommandInfo ret = { 0, 0, 0,0 };
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdParms[CMDINDEX])
		{
			ret.command = getCommandType(cmdParms[0]);
			ret.lba = getLBA(cmddata.lbaIndex, cmdParms);
			//value is used for HexValue and EndLBA
			if (cmddata.valueIndex > 0)
				ret.value = getHexValue(cmddata, cmdParms);
			else if (cmddata.endLbaIndex >0 )
				ret.value = getLBA(cmddata.endLbaIndex, cmdParms);
			else
				ret.value = 0xFFFFFFFF;

			ret.size = getSize(cmddata, cmdParms);
		}
	}
	return ret;
}

unsigned int CommandParser::getCommandType(const string cmd)
{
	auto it = this->cmdMap.find(cmd);
	if (it == this->cmdMap.end())
		return CMD_NOT_SUPPORTED;
	return this->cmdMap[cmd];
}

bool CommandParser::isValidCommand(vector<string> cmdSplits)
{
	if (checkCommand(cmdSplits) == false)
		return false;
	if (checkParamNum(cmdSplits) == false)
		return false;
	if (checkValidLBA(cmdSplits) == false)
		return false;
	if (checkValidValue(cmdSplits) == false)
		return false;
	if (checkValidSize(cmdSplits) == false)
		return false;
	/*if (checkValiEndLBA(cmdSplits) == false)
		return false;*/
	return true;
}

bool CommandParser::checkCommand(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
			return true;
	}
	return false;
}

bool CommandParser::checkParamNum(vector<string> cmdSplits)
{

	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.paramnum == cmdSplits.size() - 1)
				return true;
			else
				return false;
		}

	}
	return false;
}

bool CommandParser::checkValidLBA(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.lbaIndex>0)
			{

				string lbastr = cmdSplits[cmddata.lbaIndex];
				if (lbastr.size() <= 0 || lbastr.size() > LBAMAXLENGTH)
					return false;

				if (isNumber(lbastr) == false)
					return false;
			}

			if (cmddata.endLbaIndex>0)
			{
				string lbastr = cmdSplits[cmddata.endLbaIndex];
				if (lbastr.size() <= 0 || lbastr.size() > LBAMAXLENGTH)
					return false;

				return isNumber(lbastr);
			}

			return true;
		}
	}
	return false;
}

bool CommandParser::checkValiEndLBA(vector<string> cmdSplits)
{
	/*for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.endLbaIndex>0)
			{
				string lbastr = cmdSplits[cmddata.endLbaIndex];
				return isNumber(lbastr);
			}
			else
				return true;
		}
	}
	return false;*/
	return false;
}

CommandInfo CommandParser::MakeInvalidCmdData()
{
	return CommandInfo{ CMD_NOT_SUPPORTED, 0xFFFFFFFF,0xFFFFFFFF,-1 };
}

bool CommandParser::checkValidValue(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.valueIndex>0)
			{
				string valueStr = cmdSplits[cmddata.valueIndex];
				if (valueStr.size() != VALUELENGTH)
					return false;
				return isHex(valueStr);
			}
			else
				return true; //not use value string
		}
	}
	return false;
}

void CommandParser::runCommandHelp(void)
{
	std::cout << "* TEAM_NAME: " << teamName << std::endl;
	std::cout << "* TEAM_LEADER: " << teamLeader << std::endl;
	std::cout << "* TEAM_MEMBER: " << teamMemberName << std::endl;
	for (CommandFormat data : commandlist)
	{
		std::cout << data.cmd << data.usage << std::endl;
	}
}

//size is int
bool CommandParser::checkValidSize(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.sizeIndex>0)
			{
				string sizestr = cmdSplits[cmddata.sizeIndex];
				if (sizestr[0] == '-')
				{
					return isNumber(sizestr.substr(1, sizestr.length()));
				}
				else
				{
					return isNumber(sizestr);
				}
			}
			else
				return true;
		}
	}
	return false;
}

bool CommandParser::isNumber(const std::string& str)
{

	for (char ch : str)
	{
		if (ch < '0' || ch > '9')
			return false;

	}
	return true;
}

bool CommandParser::isHex(const std::string& str)
{
	if (str[0] != '0' || str[1] != 'x')
		return false;

	for (int i = HEXSTART; i < str.length(); i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
			continue;
		else if (str[i] >= 'A' && str[i] <= 'F')
			continue;
		else
			return false;
	}
	return true;
}

unsigned int CommandParser::getLBA(int lbaIndex, const std::vector<std::string>& cmdSplits)
{
	if (lbaIndex>0 )
	{
		return getDecimal(cmdSplits[lbaIndex]);
	}
	return 0xFFFFFFFF;
}

unsigned int CommandParser::getSize(const CommandFormat& cmddata, const std::vector<std::string>& cmdSplits)
{
	if (cmddata.sizeIndex>0)
	{
		return getSignedDecimal(cmdSplits[cmddata.sizeIndex]);
	}
	return -1;
}
unsigned int  CommandParser::getDecimal(const string& str)
{

	try {
		return stoul(str);
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Invalid argument: " << e.what() << std::endl;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Out of range: " << e.what() << std::endl;
	}
	return 0xFFFFFFFF;

}

int  CommandParser::getSignedDecimal(const string& str)
{

	try {
		return stol(str);
	}
	catch (const std::invalid_argument& e) {
		std::cerr << "Invalid argument: " << e.what() << std::endl;
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Out of range: " << e.what() << std::endl;
	}
	return 0xFFFFFFFF;

}
unsigned int CommandParser::getHexValue(const CommandFormat& cmddata, const std::vector<std::string>& cmdSplits)
{

	if (cmddata.valueIndex>0 )
	{
		try {

			return std::stoul(cmdSplits[cmddata.valueIndex], nullptr, 16);
		}
		catch (const std::invalid_argument& e) {
			std::cerr << "Invalid argument: " << e.what() << std::endl;
		}
		catch (const std::out_of_range& e) {
			std::cerr << "Out of range: " << e.what() << std::endl;
		}
	}

	return 0xFFFFFFFF;
}