#include "ShellTestScenarios.h"


unsigned int TestScenario::getRandomUnsignedInt() {
	unsigned int high = static_cast<unsigned int>(std::rand()) << 16;
	unsigned int low = static_cast<unsigned int>(std::rand()) & 0xFFFF;
	return high | low;
}
bool TestScenario::readCompare(int lba, unsigned int writtenData) {
	unsigned int readData = ssd->read(lba);
	if (readData == writtenData) {
		return true;
	}
	else {
		return false;
	}
};
bool TestScenario::execute(CommandInfo cmdInfo)
{
	log->print(__FUNCTION__, "called");
	auto it = scenarioMap.find(cmdInfo.command);
	bool isSuccess = false;
	if (it != scenarioMap.end()) {
		isSuccess = it->second(); 
	}
	printScenarioResult(isSuccess);
	return isSuccess;
}

bool TestScenario::fullWriteAndReadCompare() {
	log->print(__FUNCTION__, "called");
	unsigned int writeData = DUMMY_WRITE_DATA;
	unsigned int readData = writeData;
	int curWriteLBA = LBA_MIN;
	int curReadLBA = LBA_MIN;
	queue <WrittenData> datas;
	WrittenData data;

	while (curReadLBA <= LBA_MAX)
	{
		for (int i = 0; i < 5; i++) {
			data.lba = curWriteLBA;
			data.writtenData = writeData;
			if (curWriteLBA > LBA_MAX) break;
			if (!ssd->write(data.lba, data.writtenData)) return false;
			datas.push(data);
			curWriteLBA++;
		}
		while (!datas.empty())
		{
			data = datas.front();

			if (!readCompare(data.lba, data.writtenData)) return false;
			datas.pop();
		}
		writeData++;
		if (curWriteLBA > LBA_MAX) break;
	}
	return true;
}
bool TestScenario::partialLBAWrite() {
	log->print(__FUNCTION__, "called");
	unsigned int writeData = DUMMY_WRITE_DATA;
	queue <WrittenData> datas;
	int loopcount = 30;
	for (int i = 0; i < loopcount;i++) {

		if (!ssd->write(4, writeData)) return false;
		if (!ssd->write(0, writeData)) return false;
		if (!ssd->write(3, writeData)) return false;
		if (!ssd->write(1, writeData)) return false;
		if (!ssd->write(2, writeData)) return false;

		for (int i = 0; i < 5; i++) {
			if (!readCompare(i, writeData)) return false;
		}
	}
	return true;
}

bool TestScenario::writeReadAging() {
	log->print(__FUNCTION__, "called");
	unsigned int randomData;
	int loopcount = 200;
	for (int i = 0; i < loopcount; i++) {
		randomData = getRandomUnsignedInt();
		if (!ssd->write(0, randomData)) return false;
		if (!ssd->write(99, randomData)) return false;
		if (!readCompare(0, randomData)) return false;
		if (!readCompare(99, randomData)) return false;
	}
	return true;
}

bool TestScenario::eraseWriteAging()
{
	log->print(__FUNCTION__, "called");
	unsigned int randomData;
	if (!ssd->erase(0, 3)) return false;
	const int loopcount = 30;
	for (int loop = 0; loop < loopcount; loop++) {
		for (int i = 2; i < LBA_MAX; i += 2) {
			randomData = getRandomUnsignedInt();
			if (!ssd->write(i, randomData)) return false;
			if (!ssd->write(i, randomData)) return false;
			if (!ssd->erase(i, 1)) return false;
			if (!ssd->erase(i + 1, 1)) return false;
			if (i + 2 <= LBA_MAX) {
				if (!ssd->erase(i + 2, 1)) return  false;
			}
			if (!readCompare(i, 0)) return false;
			if (!readCompare(i + 1, 0)) return false;
			if (i + 2 <= LBA_MAX) {
				if (!readCompare(i + 2, 0)) return  false;
			}
		}
	}
	return true;
}

void TestScenario::initScenarioMap() {
	scenarioMap = {
		{ CommandType::CMD_TS_FullWriteAndReadCompare, [this]() { return fullWriteAndReadCompare(); } },
		{ CommandType::CMD_TS_PartialLBAWrite,         [this]() { return partialLBAWrite(); } },
		{ CommandType::CMD_TS_WriteReadAging,          [this]() { return writeReadAging(); } },
		{ CommandType::CMD_TS_EraseWriteAging,         [this]() { return eraseWriteAging(); } }
	};
}
void TestScenario::printScenarioResult(bool isSuccess) {
	if (isSuccess) {
		log->print(__FUNCTION__, "Pass");
		std::cout << "Pass" << "\n";
	}
	else {
		log->print(__FUNCTION__, "Fail");
		std::cout << "FAIL!" << "\n";
	}
}