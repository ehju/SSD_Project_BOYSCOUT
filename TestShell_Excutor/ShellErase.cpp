#include "ShellErase.h"
bool Erase::execute(CommandInfo cmdInfo)
{
	int size = cmdInfo.size;
	int lba = cmdInfo.lba;
	if (lba > 99 || lba < 0) return false;
	if (size == 0) return true;
	const int LBA_MAX = 99;
	const int LBA_MIN = 0;
	int tempsize;
	if (size > 0) {
		tempsize = size - 1;
		if (lba + tempsize > LBA_MAX) {
			size = LBA_MAX - lba + 1;
		}
	}
	else {
		tempsize = size + 1;
		if ((lba + tempsize) < 0) {
			size = lba + 1;
			lba = 0;
		}
		else {
			lba = lba + tempsize;
			size = -size;
		}
	}
	while (size > 10) {
		if (ssd->erase(lba, 10) == false) return false;
		lba = lba + 10;
		size = size - 10;
	}

	return ssd->erase(lba, size);
}
