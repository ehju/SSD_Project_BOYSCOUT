#pragma once
struct CommandInfo
{
	unsigned int command; //CommandType
	unsigned int lba; //param1
	unsigned int value; //param2
	int size; //param3
};

enum CommandType {

	CMD_BASIC_WRITE = 0,
	CMD_BASIC_READ,
	CMD_BASIC_ERASE,
	CMD_BASIC_ERASE_RANGE,
	CMD_BASIC_EXIT,
	CMD_BASIC_HELP,
	CMD_BASIC_FULLWRITE,
	CMD_BASIC_FULLREAD,
	CMD_BASIC_FLUSH,
	CMD_TS_FullWriteAndReadCompare,
	CMD_TS_PartialLBAWrite,
	CMD_TS_WriteReadAging,
	CMD_TS_EraseWriteAging,
	CMD_MAX,
	CMD_NOT_SUPPORTED,
};
