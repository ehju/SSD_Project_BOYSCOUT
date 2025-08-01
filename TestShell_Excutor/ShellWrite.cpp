#include "ShellWrite.h"

bool Write::execute(CommandInfo cmd) {
	log->print(__FUNCTION__, "called");
	if (cmd.lba > LBA_MAX || cmd.lba < LBA_MIN) {
		log->print(__FUNCTION__, "out of range for lba");
		return false;
	}
	return ssd->write(cmd.lba, cmd.value);
}
