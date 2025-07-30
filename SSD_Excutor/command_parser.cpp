#include "command_parser.h"

using std::string;
using std::vector;
CommandInfo CommandParser::parse(int argc, char* argv[]) 
{
	vector<string> cmdSplits;
	for(int i =1; i<argc; i++)	
	{
		cmdSplits.push_back(argv[i]);
	}
	if (isValidCommand(cmdSplits)==false)
	{
		return { (unsigned int)SSDCommand::SSDCommand_INVALID, 0xFFFFFFFF, 0xFFFFFFFF };
	}
	return MakeCommandInfo(cmdSplits);
}
bool CommandParser::isValidCommand(vector<string> cmdSplits)
{
	
	if (!checkCommand(cmdSplits))
		return false;
	if (!checkParamNum(cmdSplits))
		return false;
	if (!checkValidLBA(cmdSplits))
		return false;
	if (!checkValidValue(cmdSplits))
		return false;
	return true;
}

CommandInfo CommandParser::MakeCommandInfo(std::vector<std::string> cmdSplits)
{
	CommandInfo ret = { 0, 0, 0 };
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			ret.command = cmddata.cmdIndex;		
			if (cmddata.isUseLBA)
			{
				try {
					ret.lba = stoul(cmdSplits[LBAINDEX]);
				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Invalid argument: " << e.what() << std::endl;
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Out of range: " << e.what() << std::endl;
				}
			}
			else
				ret.value = 0xFFFFFFFF;

			if (cmddata.isUseValue)
			{
				try {
					ret.value = std::stoul(cmdSplits[VALUEINDEX], nullptr, 16);
				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Invalid argument: " << e.what() << std::endl;
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Out of range: " << e.what() << std::endl;
				}
			}
			else
				ret.value = 0xFFFFFFFF;
		
		}
	}

	return ret;
}

bool CommandParser::checkCommand(const vector<string>& cmdSplits)
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
			if (cmddata.isUseLBA)
			{

				string lbastr = cmdSplits[LBAINDEX];
				if (lbastr.size() <= 0 || lbastr.size() > LBAMAXLENGTH)
					return false;

				for (char lbach : lbastr)
				{
					if (lbach < '0' || lbach > '9')
						return false;

				}
				return true;
			}
			else
				return true;
		}
	}
	return false;
}

bool CommandParser::checkValidValue(vector<string> cmdSplits)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits[CMDINDEX])
		{
			if (cmddata.isUseValue)
			{
				int valueIndex = cmddata.paramnum; //value is lastindex
				string valueStr = cmdSplits[valueIndex];
				if (valueStr.size() != VALUELENGTH)
					return false;
				else if (valueStr[0] != '0' || valueStr[1] != 'x')
					return false;
				bool result = true;
				for (int i = VALUESTART; i < VALUELENGTH; i++)
				{
					if (valueStr[i] >= '0' && valueStr[i] <= '9')
						continue;
					else if (valueStr[i] >= 'A' && valueStr[i] <= 'F')
						continue;
					else
						return false;
				}
				return true;
			}
			else
				return true; //not use value string
		}
	}
	return false;
}