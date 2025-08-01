#include "ssd.h"
#include "command_buffer_manager.h"
#include "command_interface.h"

SSD::SSD(CommandParser* commandParser) :
	commandParser{ commandParser }
{
}

void SSD::initialize()
{
	CommandBufferManager::getInstance().initialize();
}

void SSD::run(int argc, char* argv[])
{
	FileUtil::deletePrevOutputFile();

	CommandInfo commandInfo = commandParser->parse(argc, argv);

	if (CommandBufferManager::getInstance().inputCommandBuffer(commandInfo))
	{
		return;
	}

	commandFactory.CreateCommand(commandInfo.command)->execute(commandInfo);
}
