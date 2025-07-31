#include "ShellWrite.h"

bool Write::execute(CommandInfo cmd) {
	if (cmd.lba > 99 || cmd.lba < 0) return false;
	return ssd->write(cmd.lba, cmd.value);
}
