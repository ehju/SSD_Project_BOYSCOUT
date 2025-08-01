#include "ShellWrite.h"

bool Write::execute(CommandInfo cmd) {
	Logger::getInstance()->print(__FUNCTION__, "called");
	if (cmd.lba > LBA_MAX || cmd.lba < LBA_MIN) {
		Logger::getInstance()->print(__FUNCTION__, "out of range for lba");
		return false;
	}
	return ssd->write(cmd.lba, cmd.value);
}
