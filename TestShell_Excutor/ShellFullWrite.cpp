#include "ShellFullWrite.h"
bool FullWrite::execute(CommandInfo cmdInfo)
{
	unsigned int lba = cmdInfo.lba;
	unsigned int data = cmdInfo.value;
	for (int lba = 0; lba < 100; lba++) {
		if (!ssd->write(lba, data)) return false;
	}
	return true;
}
