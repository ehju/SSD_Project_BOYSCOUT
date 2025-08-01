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
	void initialize();
	virtual std::vector<DetailedCommandInfo> getCommandBufferList();
	void flush();
	bool inputCommandBuffer(CommandInfo commandInfo);
	void syncCommandBuffer();
	void clearCommandBuffer();

private:
#ifndef _DEBUG
	CommandBufferManager() = default;
#endif
	DetailedCommandInfo string2CommandBufferInfo(std::string str);
	std::string commandBufferInfo2String(unsigned int bufferIndex, DetailedCommandInfo commnadBufferInfo);

	void updateCommandBuffer();

	void optimizeCommandBuffer();
	void updateMapForCommand(DetailedCommandInfo* detailedcommandInfo);

	CommandParser commandParser;
	fs::path folderPath = fs::current_path() / "buffer";
	std::vector<bool> bufferEnbledCommand;
	std::vector<DetailedCommandInfo> commandBufferList;
	std::vector<DetailedCommandInfo> optimizedCommandBufferList;
	int checkOptimizePossible{ 0 };

	std::array<DetailedCommandInfo*, 100> mapForOptimizeCommand;
};