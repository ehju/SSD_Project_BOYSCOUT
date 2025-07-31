#include "command_buffer_manager.h"
#include "erase.h"
#include "write.h"
#include "read.h"
#include "command_factory.h"
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

CommandBufferManager& CommandBufferManager::getInstance()
{
	static CommandBufferManager commandBufferManager;
	return commandBufferManager;
}

std::vector<CommandBufferInfo> CommandBufferManager::getCommandBufferList()
{
	return commandBufferList;
}

void CommandBufferManager::syncCommandBuffer()
{
	if (fs::exists(folderPath) == false)
	{

		fs::create_directory(folderPath);

		for (int i = 1; i <= 5; ++i) {
			fs::path filePath = folderPath / (std::to_string(i) + "_empty");
			std::ofstream outFile(filePath);
			outFile.close();
		}
	}

	for (const auto& entry : fs::directory_iterator("./buffer")) {
		
		commandBufferList.push_back(string2CommandBufferInfo(entry.path().filename().string()));
		
	}

}

std::string CommandBufferManager::commandBufferInfo2String(CommandBufferInfo commandBufferInfo)
{
	return "";
}

CommandBufferInfo CommandBufferManager::string2CommandBufferInfo(std::string str)
{
	std::stringstream ss(str);
	std::string item;
	std::vector<std::string> result;
	CommandFactory cmdFactory;

	while (std::getline(ss, item, '_')) {
		result.push_back(item);
	}

	if (result[1] == "empty")
	{
		return CommandBufferInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 0xFFFFFFFF, 0xFFFFFFFF, nullptr };
	}

	std::vector<char*> argv;
	for (auto& data : result) {
		argv.push_back(const_cast<char*>(data.c_str()));
	}

	CommandInfo commandInfo = commandParser.parse(argv.size(), argv.data());
	CommandBufferInfo commandBufferInfo = {commandInfo.command, commandInfo.lba, commandInfo.value, cmdFactory.CreateCommand(commandInfo.command)};

	return commandBufferInfo;
}

void CommandBufferManager::optimizeCommandBuffer()
{

}

void CommandBufferManager::updateCommandBuffer()
{
	
}

void CommandBufferManager::clearCommandBuffer()
{
	if (fs::exists(folderPath)) {
		fs::remove_all(folderPath);
	}

	commandBufferList.clear();
}

void CommandBufferManager::run()
{
	syncCommandBuffer();
}

void CommandBufferManager::flush()
{
	for (int i = 0; i < commandBufferList.size(); i++)
	{
		if (commandBufferList[i].cmd != static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID))
		{
			//readCommand.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_READ), address, static_cast<unsigned int>(0xFFFFFFFF) });
			commandBufferList[i].commandStructure->execute(CommandInfo{ commandBufferList[i].cmd, commandBufferList[i].param1, commandBufferList[i].param2 });
		}
	}

	clearCommandBuffer();
}