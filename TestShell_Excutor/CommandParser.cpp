#include <iostream>
#include "CommandParser.h"



bool CommandParser::invalidCheck(string str)
{
	for (string cmd : commandlist)
	{
		if (cmd == str)
			return true;
	}
	return false;
}

bool CommandParser::checkParamNum(string str)
{
	return false;
}
