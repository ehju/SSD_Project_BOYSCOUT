#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	

#include "command_interface.h"
#include "FileUtil.cpp"

class Read : public ICommand {
public:
	void execute(unsigned int address) override {
		FileUtil::deletePrevOutputFile();

		std::string matchedValue = getValue(address);
		FileUtil::writeOutputFile(matchedValue);
		return;
	}
	
private:
	std::string getValue(unsigned int address)
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

	const std::string NAND_FILE = "ssd_nand.txt";
};