#include "ShellFullRead.h"
bool FullRead::execute(unsigned int num1, unsigned int num2)
{
	unsigned int lba = num1;
	unsigned int data;
	for (int lba = 0; lba < 100; lba++) {
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