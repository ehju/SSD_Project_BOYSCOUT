#include <iostream>
#include "CommandParser.h"



bool CommandParser::InvalidCheck(string str)
{
	for (string cmd : commandlist)
	{
		if (cmd == str)
			return true;
	}
	return false;
}
