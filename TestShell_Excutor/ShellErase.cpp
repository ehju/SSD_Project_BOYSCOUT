#include "ShellErase.h"
bool Erase::execute(CommandInfo cmdInfo)
{
	Logger::getInstance()->print(__FUNCTION__, "called");
	int size = cmdInfo.size;
	int lba = cmdInfo.lba;
	if (lba > LBA_MAX || LBA_MIN < 0) {
		Logger::getInstance()->print(__FUNCTION__, "ERROR out of range");
		return false;
	}
	if (size == 0) {
		Logger::getInstance()->print(__FUNCTION__, "size is zero!");
		return true;
	}
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
		if (ssd->erase(lba, 10) == false) {
			Logger::getInstance()->print(__FUNCTION__, "ERROR in read  while()");
			return false;
		}
		lba = lba + 10;
		size = size - 10;
	}

	return ssd->erase(lba, size);
}
