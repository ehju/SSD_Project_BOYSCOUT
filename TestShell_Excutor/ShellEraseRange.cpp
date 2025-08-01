#include "ShellEraseRange.h"

bool EraseRange::execute(CommandInfo cmdInfo)
{
	Logger::getInstance()->print(__FUNCTION__, "called");
	unsigned int start_lba = cmdInfo.lba;
	unsigned int end_lba = cmdInfo.value;
	if (start_lba > end_lba) {
		std::swap(start_lba, end_lba);
		Logger::getInstance()->print(__FUNCTION__, "lba address swap");
	}
	if (start_lba < LBA_MIN) start_lba = LBA_MIN;
	else if (start_lba > LBA_MAX) start_lba = LBA_MAX;
	if (end_lba < LBA_MIN)  end_lba = LBA_MIN;
	else if (end_lba > LBA_MAX)     end_lba = LBA_MAX;
	int size = end_lba - start_lba + 1;
	return ssd->erase(start_lba, size);
}
