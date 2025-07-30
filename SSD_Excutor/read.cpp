#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>	

#include "command_interface.h"

class Read : public ICommand {
public:
	void execute(unsigned int address) override {
		deletePrevOutputFile();

		std::string matchedValue = getValue(address);
		writeOutputFile(matchedValue);
		return;
	}

	void writeOutputFile(std::string& matchedValue)
	{
		std::ofstream out(OUTPUT_FILE);
		if (out.is_open()) {
			out << matchedValue;
			out.close();
		}
		else {
			std::cout << "fail to create output file";
		}
	}

private:
	bool file_exists(const std::string& filename) {
		std::ifstream file(filename);
		return file.good();
	}

	int deleteFile(const std::string& fileName)
	{
		return std::remove(fileName.c_str());
	}

	std::string getValue(unsigned int address)
	{
		std::string ret = "0x00000000";
		std::string line;
		std::ifstream nand(NAND_FILE);
		if (!nand.is_open()) {
			std::cout << "fail to open : " << NAND_FILE << "\n";
		}
		else {
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
	
	void deletePrevOutputFile()
	{
		if (file_exists(OUTPUT_FILE)) {
			if (deleteFile(OUTPUT_FILE) == DELETE_SUCCESS) {
			}
			else {
				std::cout << "fail to delete " << OUTPUT_FILE << std::endl;
			}
		}
	}

	const std::string NAND_FILE = "ssd_nand.txt";
	const std::string OUTPUT_FILE = "ssd_output.txt";
	const int DELETE_SUCCESS = 0;
};