#include "ShellFullWrite.h"
bool FullWrite::execute(CommandInfo cmdInfo)
{
	log->print(__FUNCTION__, "called");
	unsigned int lba = cmdInfo.lba;
	unsigned int data = cmdInfo.value;
	for (int lba = LBA_MIN; lba <= LBA_MAX; lba++) {
		if (!ssd->write(lba, data)) {
			log->print(__FUNCTION__, "error on write %d %d", lba, data);
			return false;
		}
	}
	return true;
}
