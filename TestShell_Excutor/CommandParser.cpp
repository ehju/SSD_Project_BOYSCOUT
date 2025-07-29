#include "CommandParser.h"

bool CommandParser::invalidCommandCheck(string str)
{
	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == str)
			return true;
	}
	return false;
}

bool CommandParser::checkParamNum(vector<string> cmdSplits)
{

	for (CommandFormat cmddata : commandlist)
	{
		if (cmddata.cmd == cmdSplits.front())
		{
			if (cmddata.paramnum == cmdSplits.size() - 1)
				return true;
			else
				return false;
		}
			
	}
	return false;
}



