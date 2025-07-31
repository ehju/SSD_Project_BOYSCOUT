#pragma once
#include "ShellCommand.h"

enum SCENARIO
{
	FullWriteAndReadCompareScenario = 1,
	PartialLBAWriteScenario,
	WriteReadAgingScenario,
	EraseWriteAgingScenario,
};

class TestScenario : public ShellCommandItem {
public:
	TestScenario() {}
	TestScenario(iTS_SSD* ssd) : ssd{ ssd } {}
	bool execute(unsigned int num1, unsigned int num2) override;
private:
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();
	bool eraseWriteAging();

	unsigned int getRandomUnsignedInt();
	bool readCompare(int lba, unsigned int writtenData);
	iTS_SSD* ssd;
};
