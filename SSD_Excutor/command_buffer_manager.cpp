#include "command_buffer_manager.h"
#include "command_factory.h"
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

CommandBufferManager& CommandBufferManager::getInstance()
{
	static CommandBufferManager commandBufferManager;

	return commandBufferManager;
}

std::vector<CommandInfo> CommandBufferManager::getCommandBufferList()
{
	return commandBufferList;
}

void CommandBufferManager::syncCommandBuffer()
{
	if (fs::exists(folderPath) == false)
	{

		fs::create_directory(folderPath);

		for (int i = 0; i < NUM_COMMAND_BUFFER; ++i) {
			fs::path filePath = folderPath / (std::to_string(i) + "_empty");
			std::ofstream outFile(filePath);
			outFile.close();
		}
	}

	for (const auto& entry : fs::directory_iterator(folderPath)) {
		
		std::string filename = entry.path().filename().string();
		if (filename.length() >= 5 && filename.substr(filename.length() - 5) == "empty") {
			continue;
		}

		commandBufferList.push_back(string2CommandBufferInfo(entry.path().filename().string()));
		
	}

	fs::remove_all(folderPath);

}

std::string CommandBufferManager::commandBufferInfo2String(unsigned int bufferIndex, CommandInfo commandInfo)
{
	std::string line = "";
	std::string value = "";

	if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
	{
		std::ostringstream ss;
		ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << commandInfo.value;
		value = ss.str();
		line = std::to_string(bufferIndex) + "_" + commandParser.getCommandFormat(commandInfo.command).cmd + "_" + std::to_string(commandInfo.lba) + "_" + value;
	}
	else if (commandInfo.command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
	{
		value = std::to_string(commandInfo.value);
		line = std::to_string(bufferIndex) + "_" + commandParser.getCommandFormat(commandInfo.command).cmd + "_" + std::to_string(commandInfo.lba) + "_" + value;
	}
	else
	{
		line = std::to_string(bufferIndex) + "_empty";
	}
	return line;
}

CommandInfo CommandBufferManager::string2CommandBufferInfo(std::string str)
{
	std::stringstream ss(str);
	std::string item;
	std::vector<std::string> result;

	while (std::getline(ss, item, '_')) {
		result.push_back(item);
	}

	if (result[1] == "empty")
	{
		return CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_INVALID), 0xFFFFFFFF, 0xFFFFFFFF };
	}

	std::vector<char*> argv;
	for (auto& data : result) {
		argv.push_back(const_cast<char*>(data.c_str()));
	}

	CommandInfo commandInfo = commandParser.parse(static_cast<int>(argv.size()), argv.data());

	return commandInfo;
}

CommandInfo CommandBufferManager::updateCommandInfo(unsigned int command, unsigned int lba, unsigned int value)
{
	return CommandInfo{ command, lba, value };
}

void CommandBufferManager::updateOptimizedBufferListToOriginalBufferList()
{
	if (commandBufferList.size() > optimizedCommandBufferList.size())
	{
		commandBufferList.clear();

		for (int i = 0; optimizedCommandBufferList.size(); i++)
		{
			commandBufferList.push_back(optimizedCommandBufferList.back());
			optimizedCommandBufferList.pop_back();
		}
	}
}

bool CommandBufferManager::isEraseCommandHasMaxSize(unsigned int size)
{
	return (size == MAX_ERASE_SIZE);
}

void CommandBufferManager::optimizeCommandBuffer()
{
	for (int i = 0;i < commandBufferList.size();i++)
	{
		updateMapForCommand(&commandBufferList[i]);
	}

	CommandInfo waitingCommandInfo = CommandInfo{0, 0, 0};

	for (int i = 0;i < 100;i++)
	{

		if (mapForOptimizeCommand[i] == nullptr && checkOptimizePossible == 0)
		{
			continue;
		}

		if (checkOptimizePossible == 0)
		{
			if (mapForOptimizeCommand[i]->command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
			{
				waitingCommandInfo = updateCommandInfo(mapForOptimizeCommand[i]->command, i, 1);

				checkOptimizePossible++;
			}
			else if (mapForOptimizeCommand[i]->command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
			{
				waitingCommandInfo = updateCommandInfo(mapForOptimizeCommand[i]->command, mapForOptimizeCommand[i]->lba, mapForOptimizeCommand[i]->value);

				optimizedCommandBufferList.push_back(waitingCommandInfo);
				waitingCommandInfo = CommandInfo{0, 0, 0};
			}
		}
		else if (checkOptimizePossible == 1)
		{
			if (mapForOptimizeCommand[i] == nullptr)
			{
				optimizedCommandBufferList.push_back(waitingCommandInfo);
				waitingCommandInfo = CommandInfo{0, 0, 0};
				checkOptimizePossible = 0;
			}
			else if (mapForOptimizeCommand[i]->command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
			{
				waitingCommandInfo.value++;

			}
			else if (mapForOptimizeCommand[i]->command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
			{
				CommandInfo noOptimizedInfo = updateCommandInfo(mapForOptimizeCommand[i]->command, mapForOptimizeCommand[i]->lba, mapForOptimizeCommand[i]->value);
				
				waitingCommandInfo.value++;
				optimizedCommandBufferList.push_back(noOptimizedInfo);
				checkOptimizePossible++;
			}

			if (mapForOptimizeCommand[i] != nullptr)
			{
				if (isEraseCommandHasMaxSize(waitingCommandInfo.value))
				{
					optimizedCommandBufferList.push_back(waitingCommandInfo);
					waitingCommandInfo = CommandInfo{0, 0, 0};
					checkOptimizePossible = 0;
				}
			}
		}
		else if (checkOptimizePossible == 2)
		{
			if (mapForOptimizeCommand[i] == nullptr)
			{
				optimizedCommandBufferList.push_back(waitingCommandInfo);
				waitingCommandInfo = CommandInfo{0, 0, 0};
				checkOptimizePossible = 0;
			}
			else if (mapForOptimizeCommand[i]->command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
			{
				waitingCommandInfo.value++;
			}
			else if (mapForOptimizeCommand[i]->command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
			{
				CommandInfo noOptimizedInfo = updateCommandInfo(mapForOptimizeCommand[i]->command, mapForOptimizeCommand[i]->lba, mapForOptimizeCommand[i]->value);

				optimizedCommandBufferList.push_back(noOptimizedInfo);

				optimizedCommandBufferList.push_back(waitingCommandInfo);
				waitingCommandInfo = CommandInfo{0, 0, 0};
				checkOptimizePossible = 0;
			}

			if (mapForOptimizeCommand[i] != nullptr)
			{
				if (isEraseCommandHasMaxSize(waitingCommandInfo.value))
				{
					optimizedCommandBufferList.push_back(waitingCommandInfo);
					waitingCommandInfo = CommandInfo{0, 0, 0};
					checkOptimizePossible = 0;
				}
			}
		}
	}

	updateOptimizedBufferListToOriginalBufferList();
}

void CommandBufferManager::updateMapForCommand(CommandInfo* commandInfo)
{
	if (commandInfo->command == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE))
	{
		mapForOptimizeCommand[commandInfo->lba] = commandInfo;
	}
	else if (commandInfo->command == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
	{
		for (unsigned int i = commandInfo->lba; i < commandInfo->lba + commandInfo->value; i++)
		{
			mapForOptimizeCommand[i] = commandInfo;
		}
	}
}

void CommandBufferManager::updateCommandBuffer()
{
	fs::create_directory(folderPath);

	for (int bufferIdx = 0; bufferIdx < NUM_COMMAND_BUFFER; bufferIdx++)
	{
		std::string line = "";
		if (bufferIdx < commandBufferList.size())
		{
			line = commandBufferInfo2String(bufferIdx, commandBufferList[bufferIdx]);
		}
		else
		{
			line = std::to_string(bufferIdx) + "_empty";
		}

		fs::path filePath = folderPath / (line);
		std::ofstream outFile(filePath);
		outFile.close();
	}
}

void CommandBufferManager::clearCommandBuffer()
{
	if (fs::exists(folderPath)) {
		fs::remove_all(folderPath);
	}

	commandBufferList.clear();
}

bool CommandBufferManager::inputCommandBuffer(CommandInfo commandInfo)
{
	syncCommandBuffer();

	if (bufferEnbledCommand[commandInfo.command] == false)
	{
		updateCommandBuffer();
		return false;
	}

	if (commandBufferList.size() == NUM_COMMAND_BUFFER)
	{
		flush();
		clearCommandBuffer();
	}

	commandBufferList.push_back(commandInfo);

	optimizeCommandBuffer();

	updateCommandBuffer();

	return true;
}

void CommandBufferManager::flush()
{
	CommandFactory cmdFactory;
	for (int i = 0; i < commandBufferList.size(); i++)
	{
		std::shared_ptr<ICommand> command = cmdFactory.CreateCommand(commandBufferList[i].command);
		command->execute(commandBufferList[i]);
	}

	clearCommandBuffer();
}

void CommandBufferManager::initialize()
{
	commandBufferList.clear();
	optimizedCommandBufferList.clear();

	for (int i = 0; i < 100; i++)
	{
		mapForOptimizeCommand[i] = nullptr;
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(SSDCommand::SSDCommand_Count); i++)
	{
		if (i == static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE) || i == static_cast<unsigned int>(SSDCommand::SSDCommand_ERASE))
		{
			bufferEnbledCommand.push_back(true);
		}
		else
		{
			bufferEnbledCommand.push_back(false);
		}
	}

	checkOptimizePossible = 0;
}