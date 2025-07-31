#pragma once
#include "ShellCommand.h"
#include "ShellWrite.h"
#include "ShellFullWrite.h"
#include "ShellRead.h"
#include "ShellFullRead.h"
#include "ShellFlush.h"
#include "ShellErase.h"
#include "ShellTestScenarios.h"
#include "ShellEraseRange.h"

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
	CommandInfo cmdInfo{};
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
	CommandInfo cmdInfo;
	cmdInfo.size = size;
	cmdInfo.lba = lba;
	ShellCommandItem* cmd = new Erase(ssd);
	return cmd->execute(cmdInfo);
}

bool ShellCommand::erase_range(int start_lba, int end_lba)
{
	CommandInfo cmdInfo;
	cmdInfo.lba = start_lba;
	cmdInfo.value = end_lba;
	ShellCommandItem* cmd = new EraseRange(ssd);
	return cmd->execute(cmdInfo);
}

bool ShellCommand::flush()
{
	CommandInfo cmdInfo{};
	ShellCommandItem* cmd = new Flush(ssd);
	return cmd->execute(cmdInfo);
}
