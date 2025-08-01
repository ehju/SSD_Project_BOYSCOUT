#include "ShellFullWrite.h"
bool FullWrite::execute(CommandInfo cmdInfo)
{
	Logger::getInstance()->print(__FUNCTION__, "called");
	unsigned int lba = cmdInfo.lba;
	unsigned int data = cmdInfo.value;
	bool ret =false;
	for (int lba = LBA_MIN; lba <= LBA_MAX; lba++) {

		if (!ssd->write(lba, data)) {
			Logger::getInstance()->print(__FUNCTION__, "error on write %d %d", lba, data);
			return false;
		}
	}
	printWriteDone();
	return true;
}
void FullWrite::printWriteDone() {
	std::cout << "[Write] Done" << "\n";
}
