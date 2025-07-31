#include "ShellEraseRange.h"

bool EraseRange::execute(CommandInfo cmdInfo)
{
	unsigned int start_lba = cmdInfo.lba;
	unsigned int end_lba = cmdInfo.value;
	const int LBA_MAX = 99;
	const int LBA_MIN = 0;
	if (start_lba > end_lba) {
		std::swap(start_lba, end_lba);
	}
	if (start_lba < LBA_MIN) start_lba = LBA_MIN;
	else if (start_lba > LBA_MAX) start_lba = LBA_MAX;
	if (end_lba < LBA_MIN)  end_lba = LBA_MIN;
	else if (end_lba > LBA_MAX)     end_lba = LBA_MAX;
	int size = end_lba - start_lba + 1;
	return ssd->erase(start_lba, size);
}
