#include "ssd.h"
#include "command_buffer_manager.h"


SSD::SSD(CommandParser* commandParser) :
	commandParser{ commandParser }
{
	for (int i = 0; i < static_cast<int>(SSDCommand::SSDCommand_Count); i++)
	{
		commandList.push_back(commandFactory.CreateCommand(i));
	}
}

void SSD::run(int argc, char* argv[])
{
	FileUtil::deletePrevOutputFile();

	CommandInfo commandInfo = commandParser->parse(argc, argv);

	CommandBufferManager::getInstance().run();

	if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
	{
		commandList[static_cast<int>(SSDCommand::SSDCommand_WRITE)]->execute(commandInfo);
	}
	else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_READ))
	{
		commandList[static_cast<int>(SSDCommand::SSDCommand_READ)]->execute(commandInfo);
	}
	else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
	{
		commandList[static_cast<int>(SSDCommand::SSDCommand_ERASE)]->execute(commandInfo);
	}
	else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_FLUSH))
	{
		commandList[static_cast<int>(SSDCommand::SSDCommand_FLUSH)]->execute(commandInfo);
	}
	else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID))
	{
		std::string errorStr = "ERROR";
		FileUtil::writeOutputFile(errorStr);
	}
}
