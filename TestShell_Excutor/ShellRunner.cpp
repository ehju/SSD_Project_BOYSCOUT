#include "ShellRunner.h"
#include "CommandFactory.h"

void ShellRunner::initCommandMap() {
	CommandFactory cmdMaker;
	commandMap = {
		{ CommandType::CMD_BASIC_WRITE, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_BASIC_WRITE,&ssd)},
		{ CommandType::CMD_BASIC_READ, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_BASIC_READ ,&ssd)},
		{ CommandType::CMD_BASIC_ERASE, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_BASIC_ERASE ,&ssd)},
		{ CommandType::CMD_BASIC_ERASE_RANGE, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_BASIC_ERASE_RANGE, &ssd)},
		{ CommandType::CMD_BASIC_FLUSH, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_BASIC_FLUSH, &ssd)},
		{ CommandType::CMD_BASIC_FULLWRITE, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_BASIC_FULLWRITE, &ssd)},
		{ CommandType::CMD_BASIC_FULLREAD, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_BASIC_FULLREAD, &ssd)},
		{ CommandType::CMD_TS_FullWriteAndReadCompare, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_TS_FullWriteAndReadCompare, &ssd)},
		{ CommandType::CMD_TS_PartialLBAWrite, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_TS_PartialLBAWrite, &ssd)},
		{ CommandType::CMD_TS_WriteReadAging, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_TS_WriteReadAging, &ssd)},
		{ CommandType::CMD_TS_EraseWriteAging, cmdMaker.MakeCommandItem((unsigned int)CommandType::CMD_TS_EraseWriteAging, &ssd)},
	};
}

int ShellRunner::runCommand(const string cmd) {
	if (cmd.empty())
		return -1;
	Logger::getInstance()->print(__FUNCTION__, "called");
	CommandInfo cmdInfo = cp.createCommandData(cmd);
	
	int type = cmdInfo.command;

	bool result;

	if (type == CMD_NOT_SUPPORTED) {
		return CMD_NOT_SUPPORTED;
	}
	else if (type == CMD_BASIC_EXIT) {
		return CMD_BASIC_EXIT;
	}
	else if (type == CMD_BASIC_HELP) {
		cp.runCommandHelp();
		return CMD_BASIC_HELP;
	}
	else {
		result = runShellCommand(cmdInfo);
	}
	return type;
}
bool ShellRunner::runShellCommand(CommandInfo cmdInfo) {
	Logger::getInstance()->print(__FUNCTION__, "called");
	int type = cmdInfo.command;
	bool result = false;;
	auto it = commandMap.find(static_cast <unsigned int>(cmdInfo.command));
	
	if (it != commandMap.end()) {
		result = it->second->execute(cmdInfo);
	}	
	return result;
}

int ShellRunner::runScriptFile(const char* filename) {
	std::ifstream file;

	file.open(filename);
	if (file.is_open()) {
		string command;
		while (std::getline(file, command)) {
			std::cout << command << "\t___\tRun...";

			bool result  = this->runCommand(command);
			if (result == false) {
				break;
			}
		}
		file.close();
	}
	return 0;
}

