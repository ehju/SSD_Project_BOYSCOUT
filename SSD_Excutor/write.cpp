#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip> 

#include "command_interface.h"

class Write : public ICommand {
public:
	Write()
	{
		for (int i = 0; i < SSD_SIZE_PER_LBA; i++)
		{
			map[i] = 0;
		}
	}

	void execute(unsigned int lba, unsigned int value) override {
		
		sync();
		
		map[lba] = value;

		flush();

		return;
	}

private:
	static const unsigned int START_LBA = 0;
	static const unsigned int SSD_SIZE_PER_LBA = 100;

	std::string nand = "ssd_nand.txt";
	unsigned int map[SSD_SIZE_PER_LBA];

	void sync()
	{
		std::ifstream file(nand);

		if (file.is_open())
		{
			for (int i = 0; i < SSD_SIZE_PER_LBA; i++)
			{
				std::string line = "";
				getline(file, line);

				std::string lbaStr;
				std::string valueStr;
				unsigned int lba;
				long int value;

				std::istringstream iss(line);

				iss >> lbaStr >> valueStr;
				lba = stoi(lbaStr);

				char* endptr;
				value = strtol(valueStr.c_str(), &endptr, 16);

				map[lba] = value;
			}
		}

		file.close();

	}

	void flush()
	{
		std::ofstream file(nand, std::ios::out | std::ios::trunc);

		for (int i = 0; i < SSD_SIZE_PER_LBA; i++)
		{
			std::string line = "";
			std::ostringstream ss;
			ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << map[i];
			line = std::to_string(i) + " " + ss.str() + "\n";
			file << line;

		}

		file.close();
	}
};