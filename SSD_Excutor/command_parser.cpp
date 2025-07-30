#include <iostream>
#include <string>

struct CommandInfo
{
	unsigned int command;
	unsigned int lba;
	unsigned int value;

	bool operator==(const CommandInfo& other) const {
		return command == other.command && lba == other.lba && value == other.value;
	}
};

class CommandParser
{
public:
	CommandInfo parse(int argc, char* argv[])
	{
		CommandInfo ret = { 0, 0, 0 };
		return ret;
	}
};