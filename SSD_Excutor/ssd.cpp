#include <iostream>
#include "read.cpp"
#include "write.cpp"
#include "command_parser.h"



class SSD
{
public:
	SSD(CommandParser* commandParser, Write* writeCommand, Read* readCommand) :
		commandParser{ commandParser },
		writeCommand{ writeCommand },
		readCommand{ readCommand }
	{}

	Write* writeCommand;
	Read* readCommand;
	CommandParser* commandParser;

	void run(int argc, char* argv[])
	{
		CommandInfo commandInfo = commandParser->parse(argc, argv);

		if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
		{
			writeCommand->execute(commandInfo.lba, commandInfo.value);
		}
		else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_READ))
		{
			readCommand->execute(commandInfo.lba);
		}
		else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID))
		{
			std::string errorStr = "ERROR";
			readCommand->writeOutputFile(errorStr);
		}
	}
};