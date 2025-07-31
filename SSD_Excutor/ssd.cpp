#include "ssd.h"
#include "command_buffer_manager.h"
#include "command_interface.h"

SSD::SSD(CommandParser* commandParser) :
	commandParser{ commandParser }
{
}

void SSD::run(int argc, char* argv[])
{
	FileUtil::deletePrevOutputFile();

	CommandInfo commandInfo = commandParser->parse(argc, argv);

	CommandBufferManager::getInstance().run();

	commandFactory.CreateCommand(commandInfo.command)->execute(commandInfo);
}
