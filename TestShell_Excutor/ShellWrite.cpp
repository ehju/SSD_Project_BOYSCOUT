#include "ShellWrite.h"

bool Write::execute(CommandInfo cmd) {
	Logger::getInstance()->print(__FUNCTION__, "called");
	bool ret;
	if (cmd.lba > LBA_MAX || cmd.lba < LBA_MIN) {
		Logger::getInstance()->print(__FUNCTION__, "out of range for lba");
		return false;
	}
	ret = ssd->write(cmd.lba, cmd.value);
	if (ret)
	{
		printWriteDone();
	}
	return ret;
}
void Write::printWriteDone() {
	std::cout << "[Write] Done" << "\n";
}
