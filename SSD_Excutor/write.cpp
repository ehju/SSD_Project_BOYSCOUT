#include "write.h"

Write::Write() : nand{"ssd_nand.txt"}
{
	for (int i = 0; i < SSD_SIZE_PER_LBA; i++)
	{
		map[i] = 0;
	}
}

void Write::execute(unsigned int lba, unsigned int value)
{
	sync();
	map[lba] = value;
	flush();
	return;
}

void Write::sync()
{
	std::ifstream file(nand);

	if (file.is_open())
	{
		for (int i = 0; i < SSD_SIZE_PER_LBA; i++)
		{
			std::string line = "";
			getline(file, line);

			std::istringstream iss(line);
			unsigned int lba;
			std::string valueStr;
			iss >> lba >> valueStr;

			char* endptr;
			unsigned int value;
			value = strtoul(valueStr.c_str(), &endptr, 16);

			map[lba] = value;
		}
	}

	file.close();

}

void Write::flush()
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
