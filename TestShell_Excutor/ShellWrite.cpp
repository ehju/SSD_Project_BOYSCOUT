#include "ShellWrite.h"

bool Write::execute(CommandInfo cmd) {
	if (cmd.lba > LBA_MAX || cmd.lba < LBA_MIN) return false;
	return ssd->write(cmd.lba, cmd.value);
}
