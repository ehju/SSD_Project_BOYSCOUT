#include "command_factory.h"

std::shared_ptr<ICommand> CommandFactory::CreateCommand(int commandNumber)
{
	if (commandNumber == static_cast<int>(SSDCommand::SSDCommand_WRITE))
		return std::make_shared<Write>();
	else if (commandNumber == static_cast<int>(SSDCommand::SSDCommand_READ))
		return std::make_shared<ProxyRead>(&(CommandBufferManager::getInstance()));
	else if (commandNumber == static_cast<int>(SSDCommand::SSDCommand_ERASE))
		return std::make_shared<Erase>();
	else if (commandNumber == static_cast<int>(SSDCommand::SSDCommand_FLUSH))
		return std::make_shared<Flush>();

	return std::make_shared<Invalid>();
}