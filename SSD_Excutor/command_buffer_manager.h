#pragma once
#include <vector>
#include <string>
#include "command_parser.h"
#include "command_interface.h"
#include <filesystem>

namespace fs = std::filesystem;

struct CommandBufferInfo
{
	unsigned int cmd;
	unsigned int param1;
	unsigned int param2;
	std::shared_ptr<ICommand> commandStructure;
};

class CommandBufferManager
{
public:
	std::vector<CommandBufferInfo> commandBufferList;

	static CommandBufferManager& getInstance();
	std::vector<CommandBufferInfo> getCommandBufferList();
	void flush();
	void run();
	void syncCommandBuffer();
	CommandBufferInfo string2CommandBufferInfo(std::string str);
	std::string commandBufferInfo2String(CommandBufferInfo commnadBufferInfo);
	void optimizeCommandBuffer();
	void updateCommandBuffer();
	void clearCommandBuffer();

private:
	CommandBufferManager() = default;
	CommandParser commandParser;
	fs::path folderPath = fs::current_path() / "buffer";
};