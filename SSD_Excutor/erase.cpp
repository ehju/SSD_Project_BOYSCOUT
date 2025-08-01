#include "erase.h"

void Erase::execute(CommandInfo commandInfo) {
	unsigned int eraseRange = commandInfo.value;
	for (unsigned int i = 0;i < eraseRange;i++) {
		doErase(commandInfo.lba+ i);
	}
}

void Erase::doErase(unsigned int address)
{
	static unsigned int eraseValue = static_cast<unsigned int>(0x0);
	write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), address, eraseValue });
}
