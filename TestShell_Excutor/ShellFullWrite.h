#pragma once
#include "ShellCommand.h"

class FullWrite : public ShellCommandItem {
public:
	FullWrite() {}
	FullWrite(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(unsigned int num1, unsigned int num2) override;
private:
	iTS_SSD* ssd;
};
