#pragma once
#include "ShellCommand.h"

class Erase : public ShellCommandItem {
public:
	Erase() {}
	Erase(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(unsigned int num1, unsigned int num2) override;
private:
	iTS_SSD* ssd;
};
