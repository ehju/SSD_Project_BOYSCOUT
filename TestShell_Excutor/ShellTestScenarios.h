#pragma once
#include "ShellCommand.h"
#include "ShellLogger.h"
#include <unordered_map>
#include <functional>

class TestScenario : public ShellCommandItem {
public:
	
	TestScenario(const TestScenario&) = delete;          // ���� ����
	TestScenario& operator=(const TestScenario&) = delete;

	static  TestScenario* GetInstance(iTS_SSD* ssd) {
		std::lock_guard<std::mutex> lock(mtx);
		if (instance == nullptr) {
			instance = new TestScenario(ssd);         // ���� 1ȸ�� ����
		}
		return instance;
	}
	static void ResetInstance() {
		delete instance;
		instance = nullptr;
	}
	bool execute(CommandInfo cmdInfo) override;
	unsigned int getRandomUnsignedInt();
	bool readCompare(int lba, unsigned int writtenData);
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();
	bool eraseWriteAging();

private:
	TestScenario() {}
	TestScenario(iTS_SSD* ssd) : ssd{ ssd } {
		initScenarioMap();
	}
	static TestScenario* instance;                 // �ν��Ͻ� ������
	static std::mutex mtx;



	iTS_SSD* ssd;
	const unsigned int DUMMY_WRITE_DATA = 0x12345678;
	void initScenarioMap();
	void printScenarioResult(bool isSuccess);
	using ScenarioFunc = std::function<bool()>;
	std::unordered_map<unsigned int, ScenarioFunc> scenarioMap;
};