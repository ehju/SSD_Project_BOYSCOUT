#pragma once
#include <vector>
#include <string>
#include "command_parser.h"
#include "command_interface.h"
#include <filesystem>
#include <array>

namespace fs = std::filesystem;

class CommandBufferManager
{
public:
	static CommandBufferManager& getInstance();
	void initialize();
	virtual std::vector<CommandInfo> getCommandBufferList();
	void flush();
	bool inputCommandBuffer(CommandInfo commandInfo);
	void syncCommandBuffer();
	void clearCommandBuffer();

private:
#ifndef _DEBUG
	CommandBufferManager() = default;
#endif
	const unsigned int MAX_ERASE_SIZE = 10;
	static const unsigned int NUM_COMMAND_BUFFER = 5;

	CommandParser commandParser;
	fs::path folderPath = fs::current_path() / "buffer";
	std::vector<bool> bufferEnbledCommand;
	std::vector<CommandInfo> commandBufferList;
	std::vector<CommandInfo> optimizedCommandBufferList;
	int checkOptimizePossible{ 0 };

	std::array<CommandInfo*, 100> mapForOptimizeCommand;

	CommandInfo string2CommandBufferInfo(std::string str);
	std::string commandBufferInfo2String(unsigned int bufferIndex, CommandInfo commandInfo);

	void updateCommandBuffer();

	void optimizeCommandBuffer();
	void updateMapForCommand(CommandInfo* commandInfo);
	CommandInfo updateCommandInfo(unsigned int command, unsigned int lba, unsigned int value);
	void updateOptimizedBufferListToOriginalBufferList();
	bool isEraseCommandHasMaxSize(unsigned int size);
};