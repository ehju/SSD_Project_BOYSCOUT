#include <iostream>
#include <string>

struct CommandInfo
{
	unsigned int command;
	unsigned int lba;
	unsigned int value;
};

class CommandParser
{
public:
	virtual CommandInfo parse(int argc, char* argv[])
	{
		CommandInfo ret = { 0, 0, 0 };
		return ret;
	}
};