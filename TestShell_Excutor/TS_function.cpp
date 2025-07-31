#pragma once
#include "TS_function.h"

unsigned int SSDExecutor::read(int lba) {
	cmd = SSDEXCUTE + " R " + std::to_string(lba);
	int result = std::system(cmd.c_str());
	if (result == 0) {
		throw std::exception("Error System Call read");
	}

	if (file_exists(OUTFILE)) {
		string result_str = getReadResultFromFile(OUTFILE);
		if (result_str == "ERROR") {
			std::cout << "Error Read\n ";
			return 0;
		}
		unsigned int readValue = (unsigned int)std::stoul(result_str, nullptr, 16);

		return readValue;
	}
	return 0;
}
bool SSDExecutor::write(int lba, unsigned int data) {
	cmd = SSDEXCUTE + " W " + std::to_string(lba) + " " + (toHex(data));
	int result = std::system(cmd.c_str());
	if (result == 0) {
		return false;
	}
	if (file_exists(OUTFILE)) {
		string result_str = getReadResultFromFile(OUTFILE);
		if (result_str == "ERROR") return false;
	}

	return true;
}

bool SSDExecutor::file_exists(const std::string& filename) {
	std::ifstream file(filename);
	return file.good();
}

std::string SSDExecutor::getReadResultFromFile(string filename) {
	std::ifstream file(filename);
	std::string line;
	if (file.is_open())
	{
		getline(file, line);
		return line;
	}
	return "";
}


std::string SSDExecutor::toHex(unsigned int value) {

	std::ostringstream ss;
	ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << value;
	return ss.str();
}
