#include "CommandFactory.h"


ShellCommandItem* CommandFactory::MakeCommandItem(unsigned int type, iTS_SSD* ssd)
{
	if (type == CMD_BASIC_WRITE) { return new Write(ssd); }

	else if (type == CMD_BASIC_READ) { return new Read(ssd); }

	else if (type == CMD_BASIC_ERASE) { return new Erase(ssd); }

	else if (type == CMD_BASIC_ERASE_RANGE) { return new EraseRange(ssd); }

	else if (type == CMD_BASIC_FULLWRITE) { return new FullWrite(ssd); }

	else if (type == CMD_BASIC_FULLREAD) { return new FullRead(ssd); }

	else if (type == CMD_BASIC_FLUSH) { return new Flush(ssd); }

	return new TestScenario(ssd);

}