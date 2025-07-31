#include "ShellRead.h"
bool Read::execute(unsigned int num1, unsigned int num2)
{
	unsigned int lba = num1;
	unsigned int result;
	if (lba < 0 || lba > 99) {
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
}