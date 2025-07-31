#pragma once
#include "ShellCommand.h"

class FullRead : public ShellCommandItem {
public:
	FullRead() {}
	FullRead(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(unsigned int num1, unsigned int num2) override;
private:
	void printReadResult(int lba, unsigned int value);
	iTS_SSD* ssd;
};

