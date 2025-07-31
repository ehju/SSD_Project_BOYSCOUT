#include "erase.h"

void Erase::execute(CommandInfo commandInfo) {
	unsigned int eraseValue = static_cast<unsigned int>(0x0);
	for (unsigned int i = 0;i < commandInfo.value;i++) {
		write.execute(CommandInfo{ static_cast<unsigned int>(SSDCommand::SSDCommand_WRITE), commandInfo.lba + i, eraseValue });
	}
}
