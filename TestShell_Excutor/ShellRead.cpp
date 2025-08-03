#include "ShellRead.h"
bool Read::execute(CommandInfo cmdInfo)
{
	Logger::getInstance()->print(__FUNCTION__, "called");
	unsigned int lba = cmdInfo.lba;
	unsigned int result;
	if (lba < (unsigned int)LBA_MIN || lba > (unsigned int)LBA_MAX) {
		Logger::getInstance()->print(__FUNCTION__, "ERROR out of range for read");
		return false;
	}
	result = ssd->read(lba);
	printReadResult(lba, result);
	return true;
}
void Read::printReadResult(int lba, unsigned int value)
{
	std::stringstream ss;
	ss << "0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << value;
	std::cout << "[Read] LBA " << lba << " : " << ss.str() << std::endl;
	Logger::getInstance()->print(__FUNCTION__, "[Read] LBA %d : %s ", lba, ss.str().c_str());
}