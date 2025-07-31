#include "read.h"

void Read::execute(unsigned int address) {
	FileUtil::deletePrevOutputFile();

	std::string matchedValue = getValue(address);
	FileUtil::writeOutputFile(matchedValue);
	return;
}

std::string Read::getValue(unsigned int address)
{
	std::string ret = "0x00000000";
	std::string line;
	std::ifstream nand(NAND_FILE);

	if (nand.is_open()) {
		while (std::getline(nand, line)) {
			unsigned int addr;
			std::string value;
			std::istringstream iss(line);

			if (!(iss >> addr >> value)) {
				continue;
			}

			if (addr == address) {
				ret = value;
			}
		}
		nand.close();
	}
	return ret;
}
