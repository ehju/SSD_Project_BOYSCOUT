#pragma once
#include "ShellCommand.h"

class Write : public ShellCommandItem {
public:
	Write() {}
	Write(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(CommandInfo cmdInfo) override;
private:
	iTS_SSD* ssd;
};
