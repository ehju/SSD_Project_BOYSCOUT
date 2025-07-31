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
	bool execute(CommandInfo cmdInfo) override;
	unsigned int getRandomUnsignedInt();
	bool readCompare(int lba, unsigned int writtenData);
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();
	bool eraseWriteAging();
private:



	iTS_SSD* ssd;
};
