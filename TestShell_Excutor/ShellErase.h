#pragma once
#include "ShellCommand.h"
#include "ShellLogger.h"

class Erase : public ShellCommandItem {
public:
	Erase() {}
	Erase(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(CommandInfo cmdInfo) override;
private:
	iTS_SSD* ssd;
	Logger* log;
	const int LBA_MAX = 99;
	const int LBA_MIN = 0;
};
