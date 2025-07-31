#pragma once
#include <vector>
#include <string>
#include "command_parser.h"
#include "command_interface.h"
#include <filesystem>
#include <array>

namespace fs = std::filesystem;

struct DetailedCommandInfo
{
	CommandInfo commandInfo;
	std::shared_ptr<ICommand> commandStructure;
};

class CommandBufferManager
{
public:
	static const unsigned int NUM_COMMAND_BUFFER = 5;

	static CommandBufferManager& getInstance();
	virtual std::vector<DetailedCommandInfo> getCommandBufferList();
	void flush();
	bool inputCommandBuffer(CommandInfo commandInfo);
	void syncCommandBuffer();
	DetailedCommandInfo string2CommandBufferInfo(std::string str);
	std::string commandBufferInfo2String(unsigned int bufferIndex, DetailedCommandInfo commnadBufferInfo);
	void optimizeCommandBuffer();
	void updateCommandBuffer();
	void clearCommandBuffer();
	void initialize();
	void updateMapForCommand(DetailedCommandInfo* detailedcommandInfo);

private:
#ifndef _DEBUG
	CommandBufferManager() = default;
#endif
	CommandParser commandParser;
	fs::path folderPath = fs::current_path() / "buffer";
	std::vector<bool> bufferEnbledCommand = { true, false, true, false, false, false };
	std::vector<DetailedCommandInfo> commandBufferList;
	std::vector<DetailedCommandInfo> optimizedCommandBufferList;
	int checkOptimizePossible{ 0 };

	std::array<DetailedCommandInfo*, 100> mapForOptimizeCommand;
};