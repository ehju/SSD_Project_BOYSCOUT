#pragma once
#include "ShellCommand.h"
#include "ShellLogger.h"
class FullRead : public ShellCommandItem {
public:
	FullRead() {}
	FullRead(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(CommandInfo cmdInfo) override;
private:
	void printReadResult(int lba, unsigned int value);
	iTS_SSD* ssd;
};

