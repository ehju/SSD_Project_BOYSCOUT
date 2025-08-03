#pragma once
#include "ShellCommand.h"
#include "ShellLogger.h"

class EraseRange : public ShellCommandItem {
public:
	EraseRange() {}
	EraseRange(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(CommandInfo cmdInfo) override;
private:
	iTS_SSD* ssd;
};
