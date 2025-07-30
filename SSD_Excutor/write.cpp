#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include<iomanip> 


class Write {
public:
	Write()
	{
		for (int i = 0; i < 100; i++)
		{
			map[i] = 0;
		}
	}

	void execute(unsigned int lba, unsigned int value) {

		map[lba] = value;

		std::ofstream file(nand, std::ios::out | std::ios::trunc);
		
		for (int i = 0; i < 100; i++)
		{
			std::string line = "";
			std::ostringstream ss;
			ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << map[i];
			line = std::to_string(i) + " " + ss.str() + "\n";
			file << line;

		}

		file.close();

		return;
	}

	std::string nand = "ssd_nand.txt";
	unsigned int map[100];
};