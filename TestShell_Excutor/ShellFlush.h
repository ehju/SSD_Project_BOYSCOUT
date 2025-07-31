#pragma once
#include "ShellCommand.h"

class Flush : public ShellCommandItem {
public:
	Flush() {}
	Flush(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(unsigned int num1, unsigned int num2) override;
private:
	iTS_SSD* ssd;
};
