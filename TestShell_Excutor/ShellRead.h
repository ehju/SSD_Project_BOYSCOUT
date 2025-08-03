#pragma once
#include "ShellCommand.h"
#include "ShellLogger.h"

class Read : public ShellCommandItem {
public:
	Read() {}
	Read(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(CommandInfo cmdInfo) override;
private:
	void printReadResult(int lba, unsigned int value);
	iTS_SSD* ssd;
	Logger* log;
};
