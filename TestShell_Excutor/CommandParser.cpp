#include "CommandParser.h"
#include <sstream>
using namespace std;

int CommandParser::runCommand(const string cmd) {
	// Precondition
	vector<string> cmdParms = getCommandParams(cmd);
	if (isValidCommand(cmdParms) == false)
		return CMD_NOT_SUPPORTED;

	// TODO : Run command and Print Log
	return getCommandType(cmdParms[0]);
}

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

int CommandParser::getCommandType(const string cmd)
{
	auto it = this->cmdMap.find(cmd);
	if (it == this->cmdMap.end())
		return CMD_NOT_SUPPORTED;
	return this->cmdMap[cmd];
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



