#include "ShellFullWrite.h"
bool FullWrite::execute(CommandInfo cmdInfo)
{
	unsigned int lba = cmdInfo.lba;
	unsigned int data = cmdInfo.value;
	for (int lba = LBA_MIN; lba <= LBA_MAX; lba++) {
		if (!ssd->write(lba, data)) return false;
	}
	return true;
}
