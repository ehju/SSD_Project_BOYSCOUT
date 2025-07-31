#pragma once
#include "ShellCommand.h"

class FullWrite : public ShellCommandItem {
public:
	FullWrite() {}
	FullWrite(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(CommandInfo cmdInfo) override;
private:
	iTS_SSD* ssd;
	const int LBA_MAX = 99;
	const int LBA_MIN = 0;
};
