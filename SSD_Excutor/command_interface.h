#pragma once
#include <string>

class ICommand
{
public:
	virtual void execute(unsigned int lba, unsigned int value);
	virtual void execute(unsigned int lba);
	virtual void writeOutputFile(std::string& matchedValue);
};