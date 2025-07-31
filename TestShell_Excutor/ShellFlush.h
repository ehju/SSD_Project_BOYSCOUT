#pragma once
#include "ShellCommand.h"

class Flush : public ShellCommandItem {
public:
	Flush() {}
	Flush(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(CommandInfo cmdInfo) override;
private:
	iTS_SSD* ssd;
};
