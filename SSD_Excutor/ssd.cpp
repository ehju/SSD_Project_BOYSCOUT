#include <iostream>
#include <vector>
#include "read.h"
#include "write.h"
#include "command_parser.h"
#include "FileUtil.cpp"
#include "command_factory.h"

class SSD
{
public:
	SSD(CommandParser* commandParser) :
		commandParser{ commandParser }
	{
		for (int i = 0; i < static_cast<int>(SSDCommand::SSDCommand_Count); i++)
		{
			commandList.push_back(commandFactory.CreateCommand(i));
		}
	}

	CommandParser* commandParser;
	CommandFactory commandFactory;
	std::vector<std::shared_ptr<ICommand>> commandList;

	void run(int argc, char* argv[])
	{
		FileUtil::deletePrevOutputFile();

		CommandInfo commandInfo = commandParser->parse(argc, argv);

		if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
		{
			commandList[static_cast<int>(SSDCommand::SSDCommand_WRITE)]->execute(commandInfo.lba, commandInfo.value);
		}
		else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_READ))
		{
			commandList[static_cast<int>(SSDCommand::SSDCommand_READ)]->execute(commandInfo.lba);
		}
		else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
		{
			commandList[static_cast<int>(SSDCommand::SSDCommand_ERASE)]->execute(commandInfo.lba);
		}
		else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_FLUSH))
		{
			commandList[static_cast<int>(SSDCommand::SSDCommand_FLUSH)]->execute(commandInfo.lba);
		}
		else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID))
		{
			std::string errorStr = "ERROR";
			FileUtil::writeOutputFile(errorStr);
		}
	}

};