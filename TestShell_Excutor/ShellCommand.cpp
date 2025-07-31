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
	CommandInfo cmdInfo;
	cmdInfo.command = CommandType::CMD_TS_FullWriteAndReadCompare;
	ShellCommandItem* cmd = new TestScenario(ssd);
	return cmd->execute(cmdInfo);
}
bool ShellCommand::partialLBAWrite() {
	CommandInfo cmdInfo;
	cmdInfo.command = CommandType::CMD_TS_PartialLBAWrite;
	ShellCommandItem* cmd = new TestScenario(ssd);
	return cmd->execute(cmdInfo);
}

bool ShellCommand::writeReadAging() {
	CommandInfo cmdInfo;
	cmdInfo.command = CommandType::CMD_TS_WriteReadAging;
	ShellCommandItem* cmd = new TestScenario(ssd);
	return cmd->execute(cmdInfo);
}

bool ShellCommand::eraseWriteAging()
{
	CommandInfo cmdInfo;
	cmdInfo.command = CommandType::CMD_TS_EraseWriteAging;
	ShellCommandItem* cmd = new TestScenario(ssd);
	return cmd->execute(cmdInfo);
}

unsigned int ShellCommand::read(int lba) {
	CommandInfo cmdInfo;
	cmdInfo.lba = lba;
	bool ret;
	ShellCommandItem* cmd = new Read(ssd);
	return cmd->execute(cmdInfo);
}

bool ShellCommand::write(int lba, unsigned int data) {
	CommandInfo cmdInfo;
	cmdInfo.lba = lba;
	cmdInfo.value = data;
	ShellCommandItem* cmd = new Write(ssd);
	return cmd->execute(cmdInfo);
}


vector<unsigned int> ShellCommand::fullread() {
	CommandInfo cmdInfo;
	cmdInfo.lba = 0;
	bool ret;
	vector<unsigned int> result_dummy;
	ShellCommandItem* cmd = new FullRead(ssd);
	ret = cmd->execute(cmdInfo);
	return result_dummy;
}

bool ShellCommand::fullwrite(unsigned int data) {
	CommandInfo cmdInfo;
	cmdInfo.value = data;
	ShellCommandItem* cmd = new FullWrite(ssd);
	return cmd->execute(cmdInfo);
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

bool ShellCommand::erase_range(int start_lba, int end_lba)
{
	const int LBA_MAX = 99;
	const int LBA_MIN = 0;
	if (start_lba > end_lba) {
		std::swap(start_lba, end_lba);
	}
	if (start_lba < LBA_MIN) start_lba = LBA_MIN;
	else if (start_lba > LBA_MAX) start_lba = LBA_MAX;
	if (end_lba < LBA_MIN)  end_lba = LBA_MIN;
	else if (end_lba > LBA_MAX)     end_lba = LBA_MAX;
	int size = end_lba - start_lba + 1;
	return erase(start_lba, size);
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
