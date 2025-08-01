#pragma once
#include "ShellCommand.h"
#include <unordered_map>
#include <functional>
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
	TestScenario(iTS_SSD* ssd) : ssd{ ssd } {
		initScenarioMap();
	}
	bool execute(CommandInfo cmdInfo) override;
	unsigned int getRandomUnsignedInt();
	bool readCompare(int lba, unsigned int writtenData);
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();
	bool eraseWriteAging();
private:
	iTS_SSD* ssd;
	const unsigned int DUMMY_WRITE_DATA = 0x12345678;
	const int LBA_MAX = 99;
	const int LBA_MIN = 0;
	void initScenarioMap();
	void printScenarioResult(bool isSuccess);
	using ScenarioFunc = std::function<bool()>;
	std::unordered_map<unsigned int, ScenarioFunc> scenarioMap;
};