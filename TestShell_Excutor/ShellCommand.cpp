#pragma once
#include "ShellCommand.h"
#include "ShellWrite.h"
#include "ShellFullWrite.h"
#include "ShellRead.h"
#include "ShellFullRead.h"
#include "ShellFlush.h"
#include "ShellErase.h"
#include "ShellTestScenarios.h"


bool ShellCommand::readCompare(int lba, unsigned int writtenData) {
	unsigned int readData = ssd->read(lba);
	if (readData == writtenData) {
		return true;
	}
	else {
		return false;
	}
};
bool ShellCommand::fullWriteAndReadCompare() {
	unsigned int writeData = 0x12345678;
	unsigned int readData = writeData;
	int LBA_MAX = 99;
	int LBA_MIN = 0;
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
bool ShellCommand::partialLBAWrite() {
	unsigned int writeData = 0x12345678;
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

bool ShellCommand::writeReadAging() {
	unsigned int randvalue;
	int loopcount = 200;
	for (int i = 0; i < loopcount; i++) {
		randvalue = getRandomUnsignedInt();
		if (!ssd->write(0, randvalue)) return false;
		if (!ssd->write(99, randvalue)) return false;
		if (!readCompare(0, randvalue)) return false;
		if (!readCompare(99, randvalue)) return false;
	}
	return true;
}

unsigned int ShellCommand::read(int lba) {
	if (lba < 0 || lba > 99) {
		throw std::exception("invalid LBA");
	}
	return  ssd->read(lba);
}

bool ShellCommand::write(int lba, unsigned int data) {
	ShellCommandItem* cmd = new Write(ssd);
	return cmd->execute(lba,data);
}


vector<unsigned int> ShellCommand::fullread() {
	unsigned int data;
	vector<unsigned int> result;
	for (int lba = 0; lba < 100; lba++) {
		data = (ssd->read(lba));
		result.push_back(data);
	}
	return result;
}

bool ShellCommand::fullwrite(unsigned int data) {
	ShellCommandItem* cmd = new FullWrite(ssd);
	return cmd->execute(0, data);
}

bool ShellCommand::erase(int lba, int size)
{
	if (lba > 99 || lba < 0) return false;
	if (size == 0) return true;
	const int LBA_MAX = 99;
	const int LBA_MIN = 0;
	int tempsize;
	if (size > 0) {
		tempsize = size - 1;
		if (lba + tempsize > LBA_MAX) {
			size = LBA_MAX - lba + 1;
		}
	}
	else {
		tempsize = size + 1;
		if ((lba + tempsize) < 0) {
			size = lba + 1;
			lba = 0;
		}
		else {
			lba = lba + tempsize;
			size = -size;
		}
	}
	while (size > 10) {
		if (ssd->erase(lba, 10) == false) return false;
		lba = lba + 10;
		size = size - 10;
	}

	return ssd->erase(lba, size);

}

bool ShellCommand::flush()
{
	return false;
}


unsigned int ShellCommand::getRandomUnsignedInt() {
	unsigned int high = static_cast<unsigned int>(std::rand()) << 16;
	unsigned int low = static_cast<unsigned int>(std::rand()) & 0xFFFF;
	return high | low;
}
