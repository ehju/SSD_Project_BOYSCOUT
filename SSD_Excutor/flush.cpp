#include <iostream>
#include <filesystem>
#include <string>
#include "flush.h"
#include "command_buffer_manager.h"

void Flush::execute(CommandInfo commandInfo)
{
	CommandBufferManager::getInstance().flush();
}


