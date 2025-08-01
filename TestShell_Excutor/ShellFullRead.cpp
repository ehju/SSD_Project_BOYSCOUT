#include "ShellFullRead.h"
bool FullRead::execute(CommandInfo cmdInfo)
{
	//Logger::getInstance()->print(__FUNCTION__, "called");
	unsigned int lba = cmdInfo.lba;
	unsigned int data;
	for (int lba = LBA_MIN; lba <= LBA_MAX; lba++) {
		data = (ssd->read(lba));
		printReadResult(lba, data);
	}
	return true;
}

void FullRead::printReadResult(int lba, unsigned int value)
{
	std::stringstream ss;
	ss << "0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << value;
	std::cout << "[Read] LBA " << lba << " : " << ss.str() << std::endl;
}