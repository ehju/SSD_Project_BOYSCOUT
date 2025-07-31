#pragma once
#include <vector>
#include <string>
#include "command_parser.h"
#include "command_interface.h"
#include <filesystem>

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

	std::vector<DetailedCommandInfo> commandBufferList;

	static CommandBufferManager& getInstance();
	std::vector<DetailedCommandInfo> getCommandBufferList();
	void flush();
	void run();
	void syncCommandBuffer();
	DetailedCommandInfo string2CommandBufferInfo(std::string str);
	std::string commandBufferInfo2String(DetailedCommandInfo commnadBufferInfo);
	void optimizeCommandBuffer();
	void updateCommandBuffer();
	void clearCommandBuffer();

private:
	CommandBufferManager() = default;
	CommandParser commandParser;
	fs::path folderPath = fs::current_path() / "buffer";
};