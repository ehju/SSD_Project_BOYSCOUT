#pragma once
#include "gmock/gmock.h"

#include "CommandData.h"
#include "CommandParser.h"
#include "ShellCommand.h"
#include "ShellErase.h"
#include "ShellEraseRange.h"
#include "ShellFlush.h"
#include "ShellFullRead.h"
#include "ShellFullWrite.h"
#include "ShellRead.h"
#include "ShellTestScenarios.h"
#include "ShellWrite.h"
#include "ShellLogger.h"

#ifdef _DEBUG
class MockRunnerSSD : public iTS_SSD {
public:
	MOCK_METHOD(unsigned int, read, (int lba), (override));
	MOCK_METHOD(bool, write, (int lba, unsigned int data), (override));
	MOCK_METHOD(bool, erase, (int lba, int size), (override));
	MOCK_METHOD(bool, flush, (), (override));
};
#endif

class ShellRunner {
public:
	ShellRunner() { initCommandMap(); };

	int runCommand(const string cmd);
	int runScriptFile(const char* filename);
private:
#ifdef _DEBUG
	testing::NiceMock<MockRunnerSSD> ssd;
#else
	SSDExecutor ssd;
#endif
	bool runShellCommand(CommandInfo cmdInfo);
	void initCommandMap();
	CommandParser cp;


	std::unordered_map<unsigned int, ShellCommandItem*> commandMap;
//	ShellCommand shell{ &ssd };

	//Read read{ &ssd };
	//Write write{ &ssd };
	//Erase erase{ &ssd };
	//EraseRange eraserange{ &ssd };
	//Flush flush{ &ssd };
	//FullWrite fullwrite{ &ssd };
	//FullRead fullread{ &ssd };
	//TestScenario ts{ &ssd };
};