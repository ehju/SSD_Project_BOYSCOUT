#include "ShellRunner.h"


int ShellRunner::runCommand(const string cmd) {
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
	int type = cmdInfo.command;
	bool result;

	if (type == CMD_BASIC_WRITE) {
		result = write.execute(cmdInfo);
	}
	else if (type == CMD_BASIC_READ) {
		result = read.execute(cmdInfo);
	}
	else if (type == CMD_BASIC_ERASE) {
		result = erase.execute(cmdInfo);
	}
	else if (type == CMD_BASIC_ERASE_RANGE) {
		result = eraserange.execute(cmdInfo);
	}
	else if (type == CMD_BASIC_FULLWRITE) {
		result = fullwrite.execute(cmdInfo);
	}
	else if (type == CMD_BASIC_FULLREAD) {
		result = fullread.execute(cmdInfo);
	}
	else { // CMD_TS_*
		result = ts.execute(cmdInfo);
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